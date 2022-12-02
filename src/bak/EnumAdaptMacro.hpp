#ifndef ENUM_ADAPT_MACRO_HPP
#define ENUM_ADAPT_MACRO_HPP


//macro symbol split

#define __FUNC_S(a, b, c, d, e)  a##b##c##d##e
#define __FUNC_ENUM_TO_STR(a,b)  __FUNC_S(a, b, , ,)

#define  __CLASS_STRING2ENUM(enumType, symbol_func_,className, strValue) enumType className::__FUNC_S(symbol_func_, enumType,,,)(const std::string& strValue)
#define __CLASS_ENUM_TO_STR(className, symbol_fun, enumType, enumValue) std::string className::__FUNC_S(symbol_fun,,,,)(enumType enumValue)

// #define CLASS_STRING2ENUM(enumType) __CLASS_STRING2ENUM(EnumCvt, Enum2String, enumType, enumInput)
// #define CLASS_ENUM_TO_STR(enumType) __CLASS_ENUM_TO_STR(enumType, String2Enum_, EnumCvt, strEnum)

// #define __CLASS_ENUM_TO_STR(enumType, symbol_func_,className, strValue) enumType className::__FUNC_ENUM_TO_STR(symbol_func_, enumType)(const std::string& strValue)
// #define __CLASS_STRING2ENUM(className, symbol_fun, enumType, enumValue) std::string className::__FUNC_S(symbol_fun,,,,)(enumType enumValue)
 
#define CLASS_STRING2ENUM(enumType) __CLASS_STRING2ENUM(enumType, String2Enum_, EnumCvt, strEnum)
#define CLASS_ENUM_TO_STR(enumType) __CLASS_ENUM_TO_STR(EnumCvt, Enum2String, enumType, enumInput)
#define DECLARE_CLASS_FUNC_STRING2ENUM(enumType)  CLASS_STRING2ENUM(enumType);
#define DECLARE_CLASS_ENUM_TO_STR(enumType)  CLASS_ENUM_TO_STR(enumType);
#define DECLARE_CLASS_MEM_FUNC_STRING2ENUM(enumType) enumType __FUNC_S(String2Enum_, enumType,,,)(const std::string& strEnum);
#define DECLARE_CLASS_MEM_ENUM_TO_STR(enumType) std::string __FUNC_S(Enum2String,,,,)(enumType enumInput);

#define FUNC_STRING2ENUM_2(enumType, enumValue_1) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else {LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1);}\
}
#define FUNC_STRING2ENUM_3(enumType, enumValue_1, enumValue_2) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else {LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1);}\
}
#define FUNC_STRING2ENUM_4(enumType, enumValue_1, enumValue_2, enumValue_3) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else {LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1);}\
}
#define FUNC_STRING2ENUM_5(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
#define FUNC_STRING2ENUM_6(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_5;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
#define FUNC_STRING2ENUM_7(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_5;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_6;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
#define FUNC_STRING2ENUM_8(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_5;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_6)){return enumValue_6;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_7)){return enumValue_7;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
#define FUNC_STRING2ENUM_9(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_5;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_6)){return enumValue_6;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_7)){return enumValue_7;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_8)){return enumValue_8;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
#define FUNC_STRING2ENUM_10(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8,enumValue_9) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_5;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_6)){return enumValue_6;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_7)){return enumValue_7;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_8)){return enumValue_8;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_9)){return enumValue_9;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
#define FUNC_STRING2ENUM_11(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8,enumValue_9, enumValue_10) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_5;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_6)){return enumValue_6;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_7)){return enumValue_7;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_8)){return enumValue_8;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_9)){return enumValue_9;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_10)){return enumValue_10;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
#define FUNC_STRING2ENUM_12(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8,enumValue_9, enumValue_10, enumValue_11) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_5;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_6)){return enumValue_6;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_7)){return enumValue_7;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_8)){return enumValue_8;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_9)){return enumValue_9;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_10)){return enumValue_10;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_11)){return enumValue_11;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
#define FUNC_STRING2ENUM_13(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8,enumValue_9, enumValue_10, enumValue_11, enumValue_12) CLASS_STRING2ENUM(enumType){\
if (0 == strcmp(strEnum.c_str(), #enumValue_1)) {return enumValue_1;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_2)){return enumValue_2;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_3)){return enumValue_3;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_4)){return enumValue_4;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_5)){return enumValue_5;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_6)){return enumValue_6;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_7)){return enumValue_7;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_8)){return enumValue_8;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_9)){return enumValue_9;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_10)){return enumValue_10;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_11)){return enumValue_11;}\
else if(0 == strcmp(strEnum.c_str(), #enumValue_12)){return enumValue_12;}\
else { LOG_ERROR("Input %s strEnum Error!\n", #enumType);exit(1); }\
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                  EnumCvt
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FUNC_ENUM_TO_STR_1
#define FUNC_ENUM_TO_STR_2(enumType, enumValue_1) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else {LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1);}\
}
#define FUNC_ENUM_TO_STR_3(enumType, enumValue_1, enumValue_2) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_4(enumType, enumValue_1, enumValue_2, enumValue_3) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_5(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_6(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else if(enumValue_5 == enumInput) return #enumValue_5;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_7(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else if(enumValue_5 == enumInput) return #enumValue_5;\
else if(enumValue_6 == enumInput) return #enumValue_6;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_8(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else if(enumValue_5 == enumInput) return #enumValue_5;\
else if(enumValue_6 == enumInput) return #enumValue_6;\
else if(enumValue_7 == enumInput) return #enumValue_7;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_9(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else if(enumValue_5 == enumInput) return #enumValue_5;\
else if(enumValue_6 == enumInput) return #enumValue_6;\
else if(enumValue_7 == enumInput) return #enumValue_7;\
else if(enumValue_8 == enumInput) return #enumValue_8;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_10(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8, enumValue_9) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else if(enumValue_5 == enumInput) return #enumValue_5;\
else if(enumValue_6 == enumInput) return #enumValue_6;\
else if(enumValue_7 == enumInput) return #enumValue_7;\
else if(enumValue_8 == enumInput) return #enumValue_8;\
else if(enumValue_9 == enumInput) return #enumValue_9;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_11(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8, enumValue_9, enumValue_10) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else if(enumValue_5 == enumInput) return #enumValue_5;\
else if(enumValue_6 == enumInput) return #enumValue_6;\
else if(enumValue_7 == enumInput) return #enumValue_7;\
else if(enumValue_8 == enumInput) return #enumValue_8;\
else if(enumValue_9 == enumInput) return #enumValue_9;\
else if(enumValue_10 == enumInput) return #enumValue_10;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define FUNC_ENUM_TO_STR_12(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8, enumValue_9, enumValue_10, enumValue_11) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else if(enumValue_5 == enumInput) return #enumValue_5;\
else if(enumValue_6 == enumInput) return #enumValue_6;\
else if(enumValue_7 == enumInput) return #enumValue_7;\
else if(enumValue_8 == enumInput) return #enumValue_8;\
else if(enumValue_9 == enumInput) return #enumValue_9;\
else if(enumValue_10 == enumInput) return #enumValue_10;\
else if(enumValue_11 == enumInput) return #enumValue_11;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}

#define FUNC_ENUM_TO_STR_13(enumType, enumValue_1, enumValue_2, enumValue_3, enumValue_4, enumValue_5, enumValue_6, enumValue_7, enumValue_8, enumValue_9, enumValue_10, enumValue_11, enumValue_12) CLASS_ENUM_TO_STR(enumType){\
if (enumValue_1 == enumInput) return #enumValue_1;\
else if(enumValue_2 == enumInput) return #enumValue_2;\
else if(enumValue_3 == enumInput) return #enumValue_3;\
else if(enumValue_4 == enumInput) return #enumValue_4;\
else if(enumValue_5 == enumInput) return #enumValue_5;\
else if(enumValue_6 == enumInput) return #enumValue_6;\
else if(enumValue_7 == enumInput) return #enumValue_7;\
else if(enumValue_8 == enumInput) return #enumValue_8;\
else if(enumValue_9 == enumInput) return #enumValue_9;\
else if(enumValue_10 == enumInput) return #enumValue_10;\
else if(enumValue_11 == enumInput) return #enumValue_11;\
else if(enumValue_12 == enumInput) return #enumValue_12;\
else { LOG_ERROR("Input %s enum Error!\n", #enumType);exit(1); }\
}
#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, NAME, ...) NAME
#define FUNC_ENUM_TO_STR(...)                             \
    GET_MACRO(__VA_ARGS__,FUNC_ENUM_TO_STR_13, FUNC_ENUM_TO_STR_12, FUNC_ENUM_TO_STR_11, FUNC_ENUM_TO_STR_10, FUNC_ENUM_TO_STR_9, FUNC_ENUM_TO_STR_8, FUNC_ENUM_TO_STR_7, FUNC_ENUM_TO_STR_6,FUNC_ENUM_TO_STR_5, FUNC_ENUM_TO_STR_4, FUNC_ENUM_TO_STR_3, FUNC_ENUM_TO_STR_2, FUNC_ENUM_TO_STR_1) \
    (__VA_ARGS__)
#define FUNC_STRING2ENUM(...)                             \
    GET_MACRO(__VA_ARGS__, FUNC_STRING2ENUM_13, FUNC_STRING2ENUM_12, FUNC_STRING2ENUM_11, FUNC_STRING2ENUM_10, FUNC_STRING2ENUM_9, FUNC_STRING2ENUM_8, FUNC_STRING2ENUM_7, FUNC_STRING2ENUM_6,FUNC_STRING2ENUM_5, FUNC_STRING2ENUM_4, FUNC_STRING2ENUM_3, FUNC_STRING2ENUM_2, FUNC_STRING2ENUM_1) \
    (__VA_ARGS__)

#define FUNC_ENUM_CVT(...) \
    FUNC_ENUM_TO_STR(__VA_ARGS__);\
    FUNC_STRING2ENUM(__VA_ARGS__);

#define DECLARE_CLASS_MEM_ENUM_CVT(enumType) \
    DECLARE_CLASS_MEM_FUNC_STRING2ENUM(enumType) \
    DECLARE_CLASS_MEM_ENUM_TO_STR(enumType)

#endif