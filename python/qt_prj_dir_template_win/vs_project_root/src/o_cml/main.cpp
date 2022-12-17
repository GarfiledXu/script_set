#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "FQ.h"
#include "LM.h"
#include "FaceAttribution.h"
#include <unordered_set>
#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1
#include <experimental/filesystem>
#include <queue>
#include <sys/stat.h>
#include <filesystem>
#include <io.h>
#include "Environment.h"
#include "ImageConvert.h"
#include "me.hpp"
//#include <crtdbg.h>
using namespace rapidjson;
#if 1
ostream& operator<<(ostream& output, MRECT* rect) {
	output << rect->left << "," << rect->top << "," << rect->right << "," << rect->bottom;
	return output;
}
void operator<<(string& s, MRECT* rect) {
	s += to_string(rect->left);
	s += ",";
	s += to_string(rect->top);
	s += ",";
	s += to_string(rect->right);
	s += ",";
	s += to_string(rect->bottom);
}

void destoryASFA_ATTRIBUTIONRESULT(ASFA_ATTRIBUTIONRESULT* p) {
	free(p->pAgeConfArray);
	p->pAgeConfArray = nullptr;
	free(p->pAgeResultArray);
	p->pAgeResultArray = nullptr;
	free(p->pGenderConfArray);
	p->pGenderConfArray = nullptr;
	free(p->pGenderResultArray);
	p->pGenderResultArray = nullptr;
	free(p->pRaceConfArray);
	p->pRaceConfArray = nullptr;
	free(p->pRaceResultArray);
	p->pRaceResultArray = nullptr;
	free(p->pGlassConfArray);
	p->pGlassConfArray = nullptr;
	free(p->pGlassResultArray);
	p->pGlassResultArray = nullptr;
	free(p->pBeardConfArray[0]);
	p->pBeardConfArray[0] = nullptr;
	free(p->pBeardResultArray[0]);
	p->pBeardResultArray[0] = nullptr;
	free(p->pBeardConfArray[1]);
	p->pBeardConfArray[1] = nullptr;
	free(p->pBeardResultArray[1]);
	p->pBeardResultArray[1] = nullptr;
	free(p->pHatConfArray);
	p->pHatConfArray = nullptr;
	free(p->pHatResultArray);
	p->pHatResultArray = nullptr;
	free(p->pMaskConfArray);
	p->pMaskConfArray = nullptr;
	free(p->pMaskResultArray);
	p->pMaskResultArray = nullptr;
	free(p->pSmileConfArray);
	p->pSmileConfArray = nullptr;
	free(p->pSmileResultArray);
	p->pSmileResultArray = nullptr;
	free(p);
	p = nullptr;
}

void createRect(std::string rect, MRECT* rtn) {
	std::string num;
	int idx;
	auto getNum = [&](int cur, int i)->int {
		num = rect.substr(cur, i - cur);
		return std::stoi(num);
	};
	idx = rect.find(",");
	rtn->left = getNum(0, idx);
	rect = rect.substr(idx + 1);
	idx = rect.find(",");
	rtn->top = getNum(0, idx);
	rect = rect.substr(idx + 1);
	idx = rect.find(",");
	rtn->right = getNum(0, idx);
	rtn->bottom = std::stoi(rect.substr(idx + 1));
}
//
//FaceDiretion is a fixed value
std::string validKey[] = { "rect","Roll","Pitch","Yaw","LightIntensity","Cover_area","BlurDegree","AgeGroup","Gender","Race","Width","Height","FaceDirection","PhotoScene" };

bool isValidKey(std::string& key) 
{
	for (auto vKey : validKey) 
	{
		if (vKey == key) 
		{
			return true;
		}
	}
	return false;
}

void PrintLibraryVersion() {
	std::cout << "Landmark version:" << std::endl;
	const ALT_LandmarkTracking_Version* landmarkVersion = ALT_GetVersion();
	std::cout << landmarkVersion->Version << std::endl << std::endl;
	std::cout << "Landmark related library version:" << std::endl;
	//ALT_PrintVersion();
	std::cout << std::endl;
	std::cout << "FaceQuality version:" << std::endl;
	const AFQMA_VERSION* fqVersion = AFQMA_GetVersion();
	std::cout << fqVersion->strVersion << std::endl << std::endl;

	std::cout << "FaceAttribution version:" << std::endl;
	const ArcSoft_FaceAttributionEstimation_Version* faVersion = ArcSoft_FaceAttributionEstimation_GetVersion();
	std::cout << faVersion->Version << std::endl << std::endl;
}

cv::Mat ReadImage(const wchar_t* filename)
{
	FILE* fp = _wfopen(filename, L"rb");
	if (!fp)
	{
		return cv::Mat::zeros(1, 1, CV_8U);
	}
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	char* buf = new char[sz];
	fseek(fp, 0, SEEK_SET);
	long n = fread(buf, 1, sz, fp);
	cv::_InputArray arr(buf, sz);
	cv::Mat img = cv::imdecode(arr, cv::IMREAD_COLOR);
	delete[] buf;
	fclose(fp);
	return img;
}

cv::Mat _ReadImage(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");
	if (!fp)
	{
        LOG_ERROR("FILE OPEN FAILED!\n");
		//std::cout << "file open failed!" << std::endl;
		return cv::Mat::zeros(1, 1, CV_8U);
	}
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	unsigned char* buf = new unsigned char[length];
	fseek(fp, 0, SEEK_SET);
	long n = fread(buf, 1, length, fp);
	//cv::_InputArray arr(buf, sz);
	long width = 0, height = 0;

	//Todo: judge whether to do byte alignment
	nameConvertHW(filename, length, width, height);
	cv::Mat img = myConvert(buf, width, height);
	delete[] buf;
	fclose(fp);
	return img;
}

void getFiles(std::string path, std::vector<std::string>& files, std::queue<std::string>& folder)
{
	intptr_t   hFile = 0;//intptr_t和uintptr_t的类型:typedef long int； typedef unsigned long int  
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)//assign方法：先将原字符串清空，然后赋予新的值作替换。  
	{
		do
		{
			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
			{
				if (fileinfo.attrib & _A_SUBDIR) {
					folder.push(p.assign(path).append("/").append(fileinfo.name));
				}
				else {
					files.push_back(p.assign(path).append("/").append(fileinfo.name));
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void CreateNet(ncnn::Net& net) {
	//filesystem::path model_name("resnet");
	filesystem::path root_path("mobv4");
	//filesystem::path root_path("models");
	//filesystem::path param_path = root_path / model_name / "ncnn_400.param";
	//filesystem::path bin_path = root_path / model_name / "ncnn_400.bin";
	filesystem::path param_path = root_path / "ncnn_mob.param";
	filesystem::path bin_path = root_path / "ncnn_mob.bin";
	//std::cout << param_path << std::endl;
	//std::cout << bin_path << std::endl;
	if (0 != net.load_param(param_path.string().c_str()))
	{
		cout << "load param failed!" << endl;
		return;
	}

	if (0 != net.load_model(bin_path.string().c_str()))
	{
		cout << "load model failed!" << endl;
		return;
	}
}
#endif
int32_t LOG_LEVEL_MASK = EXTR | INFO | DBUG | EROR | PRINT | COLOR | TYPE;
//std::recursive_mutex g_LOG_mutex;
void task_entry(int task_serial, const std::string& path, ThreadPool& threadpool) {
    //PrintLibraryVersion();
    LOG_EXTR("TASK INTO:[%d, %d]\n", threadpool.m_thread_serial_map[std::this_thread::get_id()], task_serial);
	std::string configFilePath = "configs.txt";
	std::string helperFilePath = "helper.txt";
	if (!std::experimental::filesystem::exists(configFilePath)) 
	{
        LOG_ERROR("config file does not exist\n");
		//std::cout << "config file does not exist..." << std::endl;
		system("pause");
		return;
	}

	ifstream configFile;
	configFile.open(configFilePath, ios::in);
	
	std::unordered_set<std::string> keys;
	std::string key;
	int faceDirection = 0;
	while (configFile >> key) 
	{
		//std::cout << key << std::endl;
		//Todo: add faceDirection
		int _faceDirection = 0;
		if (isValidKey(key))
		{
			keys.insert(key);
		}
		else 
		{
			try {
				_faceDirection = std::stoi(key);
			}
			catch (std::invalid_argument&) {
				_faceDirection = 0;
			}
			catch(std::out_of_range&){
				_faceDirection = 0;
			}
			faceDirection = _faceDirection;
		}
	}
    //LOG_INFO("FACEDIRECTION:%d\n", faceDirection);
	//std::cout << "FaceDirection: "<<faceDirection << std::endl;
    configFile.close();
    
    ncnn::Net net;
    CreateNet(net);
    cv::Mat img = cv::imread(path);
    if (!img.data) {
        img = _ReadImage(path);
        if (!img.data || img.cols < 2) 
        {
            LOG_ERROR("READ ERROR\n");
            exit(1);
        }
    }
    int width = img.cols;
    int height = img.rows;
    //LOG_INFO("READ IMG FILE:%s\n", path.c_str());
    CPathProcess cpath;
    cpath.GetFileStr(path);
    std::string txtFile = cpath.sametxtpath;
    //LOG_INFO("GET TXT FILE:%s\n", txtFile.c_str());
    std::ifstream readFile;
    readFile.open(txtFile, std::ios::in);
    std::vector<std::string> oriFile;
    std::string line;
    while (std::getline(readFile, line)) 
    {
        oriFile.push_back(line);
    }
    readFile.close();
    readFile.clear();
    ofstream myfile;
    myfile.open(txtFile);
    LPALT_FACE_INFOR face_infors;
    ALT_LAND_ENGINE FLT_tracker;
    bool isValidJson = true;
    bool needSave = false;
    for (int j = 0; j < oriFile.size(); j++) {
        /*if (j>=oriFile.size()) 
        {
            break;
        }*/
        //std::cout << "line:" << oriFile[j] << std::endl;
        rapidjson::Document document;
        //todo :save automatic label
        document.Parse(oriFile[j].c_str());
        if (document.HasParseError()) 
        {
            if (oriFile[j] != "") 
            {
                isValidJson = false;
            }
            //std::cout << "line #" << j + 1 << " has parse error." << std::endl;
            myfile << oriFile[j] << std::endl;
            continue;
        }
        if (!document.HasMember("AutoTag")) 
        {
            Value autoTags(kArrayType);
            document.AddMember("AutoTag", autoTags, document.GetAllocator());
        }
        if (document.HasMember("rect"))
        {
            MRECT* mrect = new MRECT;
            createRect(document["rect"].GetString(), mrect);

            auto addJsonArrayItem = [&](const std::string& value) {
                Value& a = document["AutoTag"];
                bool isAdd = false;
                for (auto it = a.Begin(); it != a.End(); it++) 
                {
                    if (it->GetString() == value) 
                    {
                        isAdd = true;
                        break;
                    }
                }
                if (!isAdd) 
                {
                    a.PushBack(Value(value.c_str(), value.size(), document.GetAllocator()), document.GetAllocator());
                }
            };
            auto writeItem = [&](const std::string& key, std::string value) 
            {
                if (keys.find(key) != keys.end())
                {
                    if (document.HasMember(key.c_str()))
                    {
                        Value& v = document[key.c_str()];
                        if (document[key.c_str()] == "Unmarked")
                        {
                            v.SetString(value.c_str(), value.size(), document.GetAllocator());
                            addJsonArrayItem(key);
                        }

                    }
                    else
                    {
                        document.AddMember(Value(key.c_str(), key.size(), document.GetAllocator()), Value(value.c_str(), value.size(), document.GetAllocator()), document.GetAllocator());
                        addJsonArrayItem(key);
                    }
                }
            };

            int orient = 0;
            if (document.HasMember("FaceDirection"))
            {
                string faceDirection = document["FaceDirection"].GetString();
                if (faceDirection != "Unmarked")
                {
                    orient = std::stoi(faceDirection);
                }

            }
            enum ALT_OrientCode orientCode = CreateOrientCode(orient);

            GetLandmarkWithRect(&face_infors, &FLT_tracker, img, mrect, &orientCode, CreateOrientPriorityCode(faceDirection));
            if (face_infors->nFace < 1)
            {
                //std::cout << "No face is detected in " << "\"" << document["rect"].GetString() << "\" box" << std::endl;
                myfile << oriFile[j] << std::endl;
                ALT_UninitTrackEngine(NULL, &FLT_tracker, &face_infors);
                continue;
            }
            auto AttributionResults = Get_FaceAttributionEstimation(face_infors, img);
            std::string photoScene = GetPhotoScene(net, img);

            writeItem("Roll", to_string(*(face_infors->roll_angle)));
            writeItem("Yaw", to_string(*(face_infors->yaw_angle)));
            writeItem("Pitch", to_string(*(face_infors->pitch_angle)));
            auto FQResults = Get_FQMultiAttribute(face_infors, 0, img);
            writeItem("BlurDegree", ConvertBlurValue(FQResults.BlurValue));
            writeItem("Cover_area", ConvertCoverValue(FQResults.CoverValue));
            writeItem("LightIntensity", ConvertIlluminationValue(FQResults.IlluminationValue));
            writeItem("AgeGroup", ConvertAgeValue(AttributionResults->pAgeResultArray[0]));
            writeItem("Gender", ConvertGenderValue(AttributionResults->pGenderResultArray[0]));
            writeItem("Race", ConvertRaceValue(AttributionResults->pRaceResultArray[0]));
            destoryASFA_ATTRIBUTIONRESULT(AttributionResults);
            ALT_UninitTrackEngine(NULL, &FLT_tracker, &face_infors);
            writeItem("PhotoScene", photoScene);
            writeItem("Width", to_string(width));
            writeItem("Height", to_string(height));
            writeItem("FaceDirection", to_string(faceDirection));
        }
        else 
        {
            //std::cout << "Not Exist \"rect\".Wrong Item!" << std::endl;
            isValidJson = false;
        }

        rapidjson::StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);
        myfile << buffer.GetString() << std::endl;
    }
    if (oriFile.size() == 0) 
    {
    
        Get_LM_face_infor(&face_infors, &FLT_tracker, img, CreateOrientPriorityCode(faceDirection));
        auto AttributionResults = Get_FaceAttributionEstimation(face_infors, img);
        if (face_infors->nFace == 0) 
        {
            myfile.close();
            remove(txtFile.c_str());
            ALT_UninitTrackEngine(NULL, &FLT_tracker, &face_infors);
            return;
        }
        for (int j = 0; j < face_infors->nFace; j++)
        {
            needSave = true;
            rapidjson::Document document;
            document.Parse("{}");
            {
                Value autoTags(kArrayType);
                document.AddMember("AutoTag", autoTags, document.GetAllocator());
            }
            auto addJsonArrayItem = [&](const std::string& value)
            {
                Value& a = document["AutoTag"];
                a.PushBack(Value(value.c_str(), value.size(), document.GetAllocator()), document.GetAllocator());
            };
            auto writeItem = [&](const std::string& key, std::string value)
            {
                if (keys.find(key) != keys.end()) {
                    document.AddMember(Value(key.c_str(), key.size(), document.GetAllocator()), Value(value.c_str(), value.size(), document.GetAllocator()), document.GetAllocator());
                    addJsonArrayItem(key);
                }
            };

            std::string rect;
            rect << (face_infors->rcFace + j);

            writeItem("rect", rect);
            writeItem("Roll", to_string(*(face_infors->roll_angle + j)));
            writeItem("Yaw", to_string(*(face_infors->yaw_angle + j)));
            writeItem("Pitch", to_string(*(face_infors->pitch_angle + j)));
            auto FQResults = Get_FQMultiAttribute(face_infors, j, img);
            writeItem("BlurDegree", ConvertBlurValue(FQResults.BlurValue));
            writeItem("Cover_area", ConvertCoverValue(FQResults.CoverValue));
            writeItem("LightIntensity", ConvertIlluminationValue(FQResults.IlluminationValue));
            writeItem("AgeGroup", ConvertAgeValue(AttributionResults->pAgeResultArray[j]));
            writeItem("Gender", ConvertGenderValue(AttributionResults->pGenderResultArray[j]));
            writeItem("Race", ConvertRaceValue(AttributionResults->pRaceResultArray[j]));
            
            std::string photoScene = GetPhotoScene(net, img);
            writeItem("PhotoScene", photoScene);
            writeItem("Width", to_string(width));
            writeItem("Height", to_string(height));
            writeItem("FaceDirection", to_string(faceDirection));

            rapidjson::StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            document.Accept(writer);
            myfile << buffer.GetString() << std::endl;
        }
        destoryASFA_ATTRIBUTIONRESULT(AttributionResults);
        ALT_UninitTrackEngine(NULL, &FLT_tracker, &face_infors);
    }
    myfile.close();
    net.clear();
    LOG_INFO("TASK DONE:[%d, %d]\n", threadpool.m_thread_serial_map[std::this_thread::get_id()], task_serial);
}
int main(int argc, char** argv) 
{
#if 1
    std::string entry_path;
	std::vector<std::string> vec_image_list_all;
	std::vector<std::string> vec_image_list_pending;
	std::vector<std::string> vec_image_list_processed{ "Img_1627451573984_0_4208X3120_3120X4208.NV21", "test_image/big_nv21/Img_1627451742091_13_4208X3120_3120x4208.NV21" };
	//std::cin >> entry_path;
	vec_image_list_pending = image_list_filter(R"(D:\VS2019\repos\ViewFaceTool\test_space\entry_dir\origin_img\3_folder)", { ".jpg", ".mp4" }, vec_image_list_processed);
	for (auto itm : vec_image_list_pending) {
		LOG_INFO("itm:%s\n", itm.c_str());
	}
    LOG_EXTR("vec size:%d\n", vec_image_list_pending.size());
#endif
    system("pause");
	ThreadPool thr_pool(1, 100, 100);
    int idx = 0;
    Clocker cl{};
    std::thread TaskLaunch([&]() {
        for (auto path : vec_image_list_pending) {
            //LOG_INFO("curr thread:%d, add taskid:%d \n", std::this_thread::get_id(), i);
            thr_pool.AddTask(std::bind(task_entry, ++idx, path, std::ref(thr_pool)));
        }
    });
    TaskLaunch.join();
    //24 32 52 56
	//std::this_thread::sleep_for(std::chrono::seconds(20));
    LOG_EXTR("load all task\n");
   
    thr_pool.WaitAll();
    int time_ms = cl.get_duration(Clocker::MILLISEC);
    LOG_EXTR("CAST TIME MS:%d\n", time_ms);
    ;
    //std::this_thread::sleep_for(std::chrono::seconds(20));
    LOG_EXTR("WaitAll finished\n");
	return 0;
	// main_test(argc, argv);
	// return 0;
}