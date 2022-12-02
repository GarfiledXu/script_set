#include "MySdk.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   tectonics_ deconstruction
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
MySdk::MySdk() {
    
}
MySdk::~MySdk() {
    
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   process
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*ReadTxt*/
std::string MySdk::ReadTxt(const char* txtPath) {
    std::ifstream inStmLicense(txtPath);
    std::string strTemp;
    std::string strOut;
    //inStmLicense >> strTemp;
    while (std::getline(inStmLicense, strTemp)){
        strOut = strOut + strTemp;
    }
    inStmLicense.close();
    return strOut;
}
/*Init*/
std::vector<int32_t> MySdk::Init(ASD_INIT_PARAM* pParam, ArcEvent eventFunc) {
    //ret vector
    std::vector<int32_t> vecRet;
    //img new
    this->pDmsMatImg = new tv::Mat;
    this->pDmsFbImg = new tv::FourccBuffer;
    this->pDmsArcImg = new ASVLOFFSCREEN;
    this->pOmsMatImg = new tv::Mat;
    this->pOmsFbImg = new tv::FourccBuffer;
    this->pOmsArcImg = new ASVLOFFSCREEN;
    //checklicense,VIN:8*F - init
    vecRet.push_back(this->CheckLicense("FFFFFFFF", this->ReadTxt("./license_xby.dat").c_str()));
    vecRet.push_back(this->InitAlgHandle(pParam, eventFunc));
    //judge module
    if((pParam->camMask&ASD_CAM_MASK_DMS) == ASD_CAM_MASK_DMS){
        LOG_INFO("mysdk init, dms module open!\n");
    }
    if((pParam->camMask&ASD_CAM_MASK_OMS) == ASD_CAM_MASK_OMS){
        LOG_INFO("mysdk init, oms module open!\n");
    }
    if(((pParam->camMask&ASD_CAM_MASK_DMS) != ASD_CAM_MASK_DMS)&&(pParam->camMask&ASD_CAM_MASK_OMS) != ASD_CAM_MASK_OMS){
        LOG_ERROR("error! ASD_CAM_MASK = %d, wrong!\n", pParam->camMask);
        exit(1);
    }
    //setting
    //this->AllSet(MYSDK_SET_MASK_SIGNAL, ASD_VEHICLE_SIGNAL_SPEED, 40, 0);
    return vecRet;
}
void MySdk::Uninit() {
    LOG_INFO("mysdk uninit start!\n");
    delete this->pDmsMatImg;
    delete this->pDmsFbImg;
    delete this->pDmsArcImg;
    delete this->pOmsMatImg;
    delete this->pOmsFbImg;
    delete this->pOmsArcImg;
    this->UninitAlgHandle();
    LOG_INFO("mysdk uninit end!\n");
}
/*Init*/
// void MySdk::AllSet(MYSDK_SET_MASK myMask, ASD_VEHICLE_SIGNAL_TYPE type, int32_t value, ASD_DETECT_MASK mask){
//     if((myMask&MYSDK_SET_MASK_SIGNAL == MYSDK_SET_MASK_SIGNAL)){
//         this->SetVehicleSignals(type, value);
//     }
//     if((myMask&MYSDK_SET_MASK_DMS_DETECT_SWITCH == MYSDK_SET_MASK_DMS_DETECT_SWITCH)){
//         this->SetDmsDetectSwitch(mask);
//     }
//     if((myMask&MYSDK_SET_MASK_OMS_DETECT_SWITCH == MYSDK_SET_MASK_OMS_DETECT_SWITCH)){
//         this->SetOmsDetectSwitch(mask);
//     }
    
// }

/*MY push frame*/
void MySdk::PushAdapt(SDK_MODULE enumInput, ASVLOFFSCREEN* pImage) {
    if(enumInput == SDK_MODULE_DMS){
        this->PushDmsFrame(pImage, FLAGS_speed);
    }
    else if(enumInput == SDK_MODULE_OMS){
        this->PushOmsFrame(pImage);
    }

}

void MySdk::AllSet(void) {

    /*set_speed*/
    //this->SetVehicleSignals(ASD_VEHICLE_SIGNAL_SPEED, FLAGS_speed);
    /*set_dms_detect_switch*/
    ASD_DETECT_MASK dmsDeteMask = 0;
    ASD_DETECT_MASK omsDeteMask = 0;
    
    json jsRead;
    jsRead = json::parse(this->ReadTxt("./switch_set.json"));
    
    //dms
    if (jsRead["ASD_MASK_DISTRACT"] == 1) {
        dmsDeteMask |= ASD_MASK_DISTRACT;
    }
    if (jsRead["ASD_MASK_DROWSY"] == 1) {
        dmsDeteMask |= ASD_MASK_DROWSY;
    }
    if (jsRead["ASD_MASK_DANGE_ACTION"] == 1) {
        dmsDeteMask |= ASD_MASK_DANGE_ACTION;
    }
    if (jsRead["ASD_MASK_NODSHAKE"] == 1) {
        dmsDeteMask |= ASD_MASK_NODSHAKE;
    }
    if (jsRead["ASD_MASK_GAZEAREA"] == 1) {
        dmsDeteMask |= ASD_MASK_GAZEAREA;
    }
     if (jsRead["ASD_MASK_EXPRESSION"] == 1) {
        dmsDeteMask |= ASD_MASK_EXPRESSION;
    }
    if (jsRead["ASD_MASK_GESTURE"] == 1) {
        dmsDeteMask |= ASD_MASK_GESTURE;
    }
    if (jsRead["ASD_MASK_HEADPOSE"]==1){
        dmsDeteMask |= ASD_MASK_HEADPOSE;
    }
    if (jsRead["ASD_MASK_AGE_GENDER"]==1){
        dmsDeteMask |= ASD_MASK_AGE_GENDER;
    }
    //oms
    if (jsRead["ASD_OMS_MASK_DANGE_ACTION"] == 1) {
        omsDeteMask |= ASD_OMS_MASK_DANGE_ACTION;
    }
    if (jsRead["ASD_OMS_MASK_EXPRESSION"] == 1) {
        omsDeteMask |= ASD_OMS_MASK_EXPRESSION;
    }
    if (jsRead["ASD_OMS_MASK_GESTURE"] == 1) {
        omsDeteMask |= ASD_OMS_MASK_GESTURE;
    }
    if (jsRead["ASD_OMS_MASK_BABY"] == 1) {
        omsDeteMask |= ASD_OMS_MASK_BABY;
    }
    if (jsRead["ASD_OMS_MASK_HUMEN_PET"] == 1) {
        omsDeteMask |= ASD_OMS_MASK_HUMEN_PET;
    }
    if (jsRead["ASD_OMS_MASK_RESIDUE"] == 1) {
        omsDeteMask |= ASD_OMS_MASK_RESIDUE;
    }
    if (jsRead["ASD_OMS_MASK_AGE_GENDER"] == 1) {
        omsDeteMask |= ASD_OMS_MASK_AGE_GENDER;
    }
    LOG_INFO("========>dmsDeteMask:%d\n", dmsDeteMask);
    LOG_INFO("========>omsDeteMask:%d\n", omsDeteMask);
    LOG_INFO("========>dmsDeteMask set ret:%d\n", this->SetDmsDetectSwitch(dmsDeteMask));
    LOG_INFO("========>omsDeteMask set ret:%d\n", this->SetOmsDetectSwitch(omsDeteMask));
    
    
    LOG_INFO("set sdk!\n");
}
