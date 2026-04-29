/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ut_ive_case.h"
#include "ut_ive_common.h"

AX_BOOL g_bAlignNeed = AX_FALSE;
AX_BOOL g_bSave = AX_FALSE;
static AX_U32 g_u32TestIndex = 0;

/******************************************************************************
* function : to process abnormal case
******************************************************************************/
AX_VOID SAMPLE_IVE_HandleSig(AX_S32 s32Signo)
{
    if (SIGINT == s32Signo || SIGTERM == s32Signo) {
        AX_U32 u32Index = g_u32TestIndex;
        if (u32Index < 0)
            return;
        switch (u32Index) {
        case 0:
            SAMPLE_IVE_DMA_TEST_HandleSig();
            break;
        case 1:
            SAMPLE_IVE_DualPicCalc_TEST_HandleSig();
            break;
        case 2:
            SAMPLE_IVE_EdgeDetection_TEST_HandleSig();
            break;
        case 3:
            SAMPLE_IVE_CCL_TEST_HandleSig();
            break;
        case 4:
            SAMPLE_IVE_ED_TEST_HandleSig();
            break;
        case 5:
            SAMPLE_IVE_Filter_TEST_HandleSig();
            break;
        case 6:
            SAMPLE_IVE_Hist_TEST_HandleSig();
            break;
        case 7:
            SAMPLE_IVE_Integ_TEST_HandleSig();
            break;
        case 8:
            SAMPLE_IVE_MagAng_TEST_HandleSig();
            break;
        case 9:
            SAMPLE_IVE_Sobel_TEST_HandleSig();
            break;
        case 10:
            SAMPLE_IVE_GMM_TEST_HandleSig();
            break;
        case 11:
            SAMPLE_IVE_Thresh_TEST_HandleSig();
            break;
        case 12:
            SAMPLE_IVE_16To8Bit_TEST_HandleSig();
            break;
        case 13:
            SAMPLE_IVE_TestMultiCalc_HandleSig();
            break;
        case 14:
            SAMPLE_IVE_CropResize_TEST_HandleSig();
            break;
        case 15:
            SAMPLE_IVE_CSC_TEST_HandleSig();
            break;
        case 16:
            SAMPLE_IVE_TestCases_ThreadStop(1);
            AXCL_IVE_Exit();
            AXCL_SYS_Deinit();
            break;
        case 17:
            SAMPLE_IVE_TestCases_ThreadStop(5);
            AXCL_IVE_Exit();
            AXCL_SYS_Deinit();
            break;
        case 18:
            SAMPLE_IVE_CropResize2_TEST_HandleSig();
            break;
        default :
            break;
        }

        SAMPLE_IVE_PRT("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }
    exit(-1);
}
/******************************************************************************
* function : show usage
******************************************************************************/

static AX_CHAR optstr[] = "?::c:e:m:t:i:o:w:h:p:a:s:d:";
static const struct option long_options[] = {
    {"calc_choice", required_argument, NULL, 'c'},
    {"engine_choice", required_argument, NULL, 'e'},
    {"mode_ctl", required_argument, NULL, 'm'},
    {"type_image", required_argument, NULL, 't'},
    {"input_path", required_argument, NULL, 'i'},
    {"out_path", required_argument, NULL, 'o'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"param_list", required_argument, NULL, 'p'},
    {"align_need", required_argument, NULL, 'a'},
    {"save", required_argument, NULL, 's'},
    {"duration", required_argument, NULL, 'd'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

AX_VOID SAMPLE_IVE_Usage(AX_CHAR* pchPrgName)
{
    printf("Usage : %s -c case_index [options]\n", pchPrgName);

    printf("\t-c | --case_index:Calc case index, default:0\n");
    printf("\t\t0-DMA.\n");
    printf("\t\t1-DualPicCalc.\n");
    printf("\t\t2-HysEdge and CannyEdge.\n");
    printf("\t\t3-CCL.\n");
    printf("\t\t4-Erode and Dilate.\n");
    printf("\t\t5-Filter.\n");
    printf("\t\t6-Hist and EqualizeHist.\n");
    printf("\t\t7-Integ.\n");
    printf("\t\t8-MagAng.\n");
    printf("\t\t9-Sobel.\n");
    printf("\t\t10-GMM and GMM2.\n");
    printf("\t\t11-Thresh.\n");
    printf("\t\t12-16bit to 8bit.\n");
    printf("\t\t13-Multi Calc.\n");
    printf("\t\t14-Crop and Resize.\n");
    printf("\t\t15-CSC.\n");
    printf("\t\t16-Single Thread Multi Cases Random Test.\n");
    printf("\t\t17-Multi Thread Multi Cases Random Test.\n");
	printf("\t\t18-CropResize2.\n");

    printf("\t-e | --engine_choice:Choose engine id, default:0\n");
    printf("\t\t0-IVE; 1-TDP; 2-VGP; 3-VPP; 4-GDC; 5-DSP; 6-NPU; 7-CPU.\n");
    printf("\t\tFor Crop and Resize case, cropimage support IVE/VGP/VPP engine, cropresize and cropresize_split_yuv support VGP/VPP engine.\n");
    printf("\t\tFor CSC case, support TDP/VGP/VPP engine.\n");
    printf("\t\tFor CropResize2 case, support VGP/VPP engine.\n");

    printf("\t-m | --mode_choice:Choose test mode, default:0\n");
    printf("\t\tFor DualPicCalc case, indicate dual pictures calculation task:\n"
           "\t\t  0-add; 1-sub; 2-and; 3-or; 4-xor; 5-mse.\n");
    printf("\t\tFor HysEdge and CannyEdge case, indicate hys edge or canny edge calculation task:\n"
           "\t\t  0-hys edge; 1-canny edge.\n");
    printf("\t\tFor Erode and Dilate case, indicate erode or dilate calculation task:\n"
           "\t\t  0-erode; 1-dilate.\n");
    printf("\t\tFor Hist and EqualizeHist case, indicate hist or equalize hist calculation task:\n"
           "\t\t  0-hist; 1-equalize hist.\n");
    printf("\t\tFor GMM and GMM2 case, indicate gmm or gmm2 calculation task:\n"
           "\t\t  0-gmm; 1-gmm2.\n");
    printf("\t\tFor Crop and Resize case, indicate cropimage, cropresize or cropresize_split_yuv calculation task:\n"
           "\t\t  0-crop image; 1-crop_resize; 2-cropresize_split_yuv.\n");
    printf("\t\tFor CropResize2 case, indicate crop_resize2 or cropresize2_split_yuv calculation task:\n"
           "\t\t  0-crop_resize2; 1-cropresize2_split_yuv.\n");

    printf("\t-t | --type_image:Image type index refer to IVE_IMAGE_TYPE_E(IVE engine) or AX_IMG_FORMAT_E(other engine)\n");
    printf("\t\tNote:\n"
        "\t\t  1. For all case, both input and output image types need to be specified in the same order as the specified input and output file order.\n"
        "\t\t  2. If no type is specified, i.e. a type value of -1 is passed in, then a legal type is specified, as qualified by the API documentation.\n"
        "\t\t  3. Multiple input and output image types, separated by spaces.\n"
        "\t\t  4. For One-dimensional data (such as AX_IVE_MEM_INFO_T type data), do not require a type to be specified.\n");

    printf("\t-i | --input_files:Input image files, if there are multiple inputs, separated by spaces.\n");
    printf("\t-o | --output_files:Output image files or dir, if there are multiple outputs, separated by spaces\n"
        "\t\tNote:for DMA, Crop Resize, blob of CCL case and CropResize2 case must be specified as directory.\n");
    printf("\t-w | --width:Image width of inputs, default:1280.\n");
    printf("\t-h | --height:Image height of inputs, default:720.\n");
    printf("\t-p | --param_list:Control parameters list or file(in json data format)\n");
    printf("\t\tNote:\n"
        "\t\t  1. Please refer to the json file in the '/opt/data/ive/' corresponding directory of each test case.\n"
        "\t\t  2. For MagAng, Multi Calc and CSC case, no need control parameters.\n");
    printf("\t-a | --align_need:Does the width/height/stride need to be aligned automatically, default:0.\n"
        "\t\t  0-no; 1-yes.\n");
    printf("\t-s | --save:Does save output result to files, default:0.\n"
        "\t\t  0-no; 1-yes.\n");
    printf("\t-d | --duration:Thread duration, default:10(s).\n");
    printf("\t-? | --help:Show usage help.\n");

}

/******************************************************************************
* function : ive sample
******************************************************************************/
int main(int argc, char *argv[])
{
    AX_S32 s32Ret;
    AX_U32 u32Index = 0;
    AX_U32 u32EngineId = 0;
    AX_U32 u32Mode = 0;
    AX_S32 as32Type[5] = {-1, -1, -1, -1, -1};
    AX_CHAR *pchPathSrc[2] = {NULL, NULL};
    AX_CHAR *pchPathDst[3] = {NULL, NULL, NULL};
    AX_U32 u32WidthSrc = 1280;
    AX_U32 u32HeightSrc = 720;
    AX_CHAR *pchParamsList = NULL;
    AX_U32 u32Duration = 10;//second

    if (argc < 2) {
        SAMPLE_IVE_Usage(argv[0]);
        return AX_FAILURE;
    }
    if(u32Index != 19) {
        signal(SIGINT, SAMPLE_IVE_HandleSig);
        signal(SIGTERM, SAMPLE_IVE_HandleSig);
    }

    while ((s32Ret = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
        if (s32Ret < 0)
            break;

        switch(s32Ret) {
        case 'c':
            u32Index = (AX_U32)atoi(optarg);
            break;
        case 'e':
            u32EngineId = (AX_U32)atoi(optarg);
            if (u32EngineId >= AX_IVE_ENGINE_BUTT) {
                SAMPLE_IVE_PRT("Engine[%d] illegal!\n", u32EngineId);
                return AX_FAILURE;
            }
        break;
        case 'm':
            u32Mode = (AX_U32)atoi(optarg);
            break;
        case 't':
            as32Type[0] = atoi(argv[optind - 1]);
            if (u32Index != 6) {
                if (optind <= (argc - 1))
                    as32Type[1] = atoi(argv[optind]);
                if (u32Index == 1 || (u32Index == 2 && u32Mode == 0)
                    || u32Index == 8 || u32Index == 10 || u32Index == 13) {
                    if ((optind + 1) <= (argc -1))
                        as32Type[2] = atoi(argv[optind + 1]);
                }
                if (u32Index == 8) {
                    if ((optind + 2) <= (argc -1))
                        as32Type[3] = atoi(argv[optind + 2]);
                }
            }
            break;
        case 'i':
            pchPathSrc[0] = argv[optind - 1];
            if (u32Index == 1 || (u32Index == 2 && u32Mode == 0)
                || u32Index == 8 || u32Index == 10 || u32Index == 18) {
                if (optind <= (argc - 1))
                    pchPathSrc[1] = argv[optind];
            }
            break;
        case 'o':
            pchPathDst[0] = argv[optind - 1];
            if (u32Index == 3 || u32Index == 8 || u32Index == 10) {
                if (optind <= (argc - 1))
                    pchPathDst[1] = argv[optind];
                if (u32Index == 10) {
                    if ((optind + 1) <= (argc -1))
                        pchPathDst[2] = argv[optind + 1];
                }
            }
            break;
        case 'w':
            u32WidthSrc = (AX_U32)atoi(optarg);
            break;
        case 'h':
            u32HeightSrc = (AX_U32)atoi(optarg);
            break;
        case 'p':
            pchParamsList = optarg;
            break;
        case 'a':
            g_bAlignNeed = (AX_BOOL)atoi(optarg);
            break;
        case 's':
            g_bSave = (AX_BOOL)atoi(optarg);
            break;
        case 'd':
            u32Duration = (AX_U32)atoi(optarg);
            break;
        case '?':
        default:
            SAMPLE_IVE_Usage(argv[0]);
            return 0;
        }
    }
    g_u32TestIndex = u32Index;

    s32Ret = axclInit(NULL);
    if (s32Ret) {
        return s32Ret;
    }

    AX_U32 device_id = 0;
    axclrtDeviceList lst;
    s32Ret = axclrtGetDeviceList(&lst);
    if (s32Ret || 0 == lst.num) {
        SAMPLE_IVE_PRT("no device is connected\r\n");
        axclFinalize();
        return s32Ret;
    }

    device_id = lst.devices[0];
    SAMPLE_IVE_PRT("device id: %d \r\n", device_id);

    s32Ret = axclrtSetDevice(device_id);
    if (s32Ret) {
        axclFinalize();
        return s32Ret;
    }

    if(u32Index != 19) {
        if (AXCL_SYS_Init() < 0) {
            SAMPLE_IVE_PRT("Sys init failed!\n");
            return AX_FAILURE;
        }

        if ((s32Ret = AXCL_IVE_Init()) < 0) {
            SAMPLE_IVE_PRT("Ive init failed, s32Ret=0x%x!\n", s32Ret);
            return AX_FAILURE;
        }
    }

    switch(u32Index) {
    case 0://DMA
        SAMPLE_IVE_DMA_TEST(as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 1://Dual picture calculate
        if (u32Mode > 5) {
            SAMPLE_IVE_PRT("Mode[%d] illegal!\n", u32Mode);
            SAMPLE_IVE_Usage(argv[0]);
            goto EXIT;
        }
        SAMPLE_IVE_DualPicCalc_TEST(u32Mode, as32Type, pchPathSrc, pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
    break;
    case 2://HysEdge and CannyEdge
        if (u32Mode > 1) {
            SAMPLE_IVE_PRT("Mode[%d] illegal!\n", u32Mode);
            SAMPLE_IVE_Usage(argv[0]);
            goto EXIT;
        }
        SAMPLE_IVE_EdgeDetection_TEST(u32Mode, as32Type, pchPathSrc, pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 3://CCL
        SAMPLE_IVE_CCL_TEST(as32Type, pchPathSrc[0], pchPathDst, u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 4://Erode and Dilate
        if (u32Mode > 1) {
            SAMPLE_IVE_PRT("Mode[%d] illegal!\n", u32Mode);
            SAMPLE_IVE_Usage(argv[0]);
            goto EXIT;
        }
        SAMPLE_IVE_ED_TEST(u32Mode, as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 5://Filter
        SAMPLE_IVE_Filter_TEST(as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 6://Hist and EqualHist
        if (u32Mode > 1) {
            SAMPLE_IVE_PRT("Mode[%d] illegal!\n", u32Mode);
            SAMPLE_IVE_Usage(argv[0]);
            goto EXIT;
        }
        SAMPLE_IVE_Hist_TEST(u32Mode, as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 7://Integ
        SAMPLE_IVE_Integ_TEST(as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 8://MagAng
        SAMPLE_IVE_MagAng_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc);
        break;
    case 9://Sobel
        SAMPLE_IVE_Sobel_TEST(as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 10://GMM and GMM2
        if (u32Mode > 1) {
            SAMPLE_IVE_PRT("Mode[%d] illegal!\n", u32Mode);
            SAMPLE_IVE_Usage(argv[0]);
            goto EXIT;
        }
        SAMPLE_IVE_GMM_TEST(u32Mode, as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 11://Thresh
        SAMPLE_IVE_Thresh_TEST(as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 12://16bit to 8bit
        SAMPLE_IVE_16To8Bit_TEST(as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 13://MultiCalc
        SAMPLE_IVE_TestMultiCalc(as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc);
        break;
    case 14://CropResize
        if (u32Mode > 2) {
            SAMPLE_IVE_PRT("Mode[%d] illegal!\n", u32Mode);
            SAMPLE_IVE_Usage(argv[0]);
            goto EXIT;
        }
        SAMPLE_IVE_CropResize_TEST(u32EngineId, u32Mode, as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 15://CSC
        SAMPLE_IVE_CSC_TEST(u32EngineId, as32Type, pchPathSrc[0], pchPathDst[0], u32WidthSrc, u32HeightSrc);
        break;
    case 16://Single Thread
        SAMPLE_IVE_TestCases_ThreadStart(1, &device_id);
        if (u32Duration > 0) {
            for (AX_S32 i = 0; i < u32Duration; i++) {
                if (!g_bCaseTestSuccess)
                    break;
                usleep(1000 * 1000);
            }
            SAMPLE_IVE_TestCases_ThreadStop(1);
        }
        break;
    case 17://Multi Thread
        SAMPLE_IVE_TestCases_ThreadStart(5, &device_id);
        if (u32Duration > 0) {
            for (AX_S32 i = 0; i < u32Duration; i++) {
                if (!g_bCaseTestSuccess)
                    break;
                usleep(1000 * 1000);
            }
            SAMPLE_IVE_TestCases_ThreadStop(5);
        }
        break;
	 case 18://CropResize2
        if (u32Mode > 1) {
            SAMPLE_IVE_PRT("Mode[%d] illegal!\n", u32Mode);
            SAMPLE_IVE_Usage(argv[0]);
            goto EXIT;
        }
        SAMPLE_IVE_CropResize2_TEST(u32EngineId, u32Mode, as32Type, pchPathSrc, pchPathDst[0], u32WidthSrc, u32HeightSrc, pchParamsList);
        break;
    case 19://Multi Process
        SAMPLE_IVE_TestCases_Process(u32Duration, 3);
        break;
    default :
        SAMPLE_IVE_PRT("No support the testing case!\n");
        SAMPLE_IVE_Usage(argv[0]);
        goto EXIT;
        break;
    }

EXIT:
    if(u32Index != 19) {
        AXCL_IVE_Exit();
        AXCL_SYS_Deinit();
    }

    axclrtResetDevice(device_id);
    axclFinalize();
    return 0;

}



