# - Find Abaqus SDK library
# Find the Abaqus SDK includes and library

find_path(AbqSDK_PRIVATE_INCLUDE_DIR
  NAMES
    odb_API.h
  PATHS
    /usr/local/include
    /usr/include
)
mark_as_advanced(AbqSDK_PRIVATE_INCLUDE_DIR)

# find *.lib from Abaqus
set(ABQ_LIB_LIST 
"ABQDMP_Core"
"ABQSMAAbuBasicUtils"
"ABQSMAAbuGeom"
"ABQSMAAspCommunications"
"ABQSMAAspDiagExtractor"
"ABQSMAAspExpKernelComm"
"ABQSMAAspSupport"
"ABQSMABasAlloc"
"ABQSMABasCoreUtils"
"ABQSMABasGenericsLib"
"ABQSMABasPrfTrkLib"
"ABQSMABasRtvUtility"
"ABQSMABasShared"
"ABQSMABasXmlDocument"
"ABQSMABasXmlParser"
"ABQSMABlaModule"
"ABQSMACseModules"
"ABQSMAEliBaseModule"
"ABQSMAEliLicenseModule"
"ABQSMAEliStaticModule"
"ABQSMAElkCore"
"ABQSMAFeoModules"
"ABQSMAFsmShared"
"ABQSMAISimInterface"
"ABQSMAISrvInterface"
"ABQSMAMsgCommModules"
"ABQSMAMsgModules"
"ABQSMAMtkApiMod"
"ABQSMAMtxCoreModule"
"ABQSMAObjSimObjectsMod"
"ABQSMAOdbApi"
"ABQSMAOdbAttrEO"
"ABQSMAOdbCalcK"
"ABQSMAOdbCore"
"ABQSMAOdbCoreGeom"
"ABQSMAOdbDdbOdb"
"ABQSMARfmInterface"
"ABQSMARomDiagEx"
"ABQSMASglSharedLib"
"ABQSMASglSimXmlFndLib"
"ABQSMAShaDbIface-D"
"ABQSMAShaDbIface"
"ABQSMAShaShared-D"
"ABQSMAShaShared"
"ABQSMAShpCore"
"ABQSMASimBCompress"
"ABQSMASimBulkAPI"
"ABQSMASimContainers"
"ABQSMASimInterface"
"ABQSMASimManifestSubcomp"
"ABQSMASimPoolManager"
"ABQSMASimS2fSubcomp"
"ABQSMASimSerializerAPI"
"ABQSMASrvBasic"
"ABQSMASrvSimXmlConverters"
"ABQSMASspUmaCore"
"ABQSMAUsubsLib"
"ABQSMAUzlZlib"
"CATBBMagic"
"CATComBase"
"CATComDrvBB"
"CATComHTTPEndPoint"
"CATComServices"
"CATComSidl"
"CATComSidlFile"
"CATLic"
"CATLMjni"
"CATP2PBaseUUID"
"CATP2PCore"
"CATPLMDispatcherItf"
"CATPLMDispatcherSpecificItf"
"CATScriptEngine"
"CATSysAllocator"
"CATSysCATIAAI"
"CATSysCommunication"
"CATSysDbSettings"
"CATSysExternApp"
"CATSysMainThreadMQ"
"CATSysMotifDrv"
"CATSysMultiThreading"
"CATSysPreview"
"CATSysProxy"
"CATSysRunBrw"
"CATSysTS"
"CATSysTSObjectModeler"
"CommunicationsUUID"
"CSICommandBinder"
"CSINodesLauncherSrc"
"CSIQueuingDatabaseModule"
"CSIQueuingModule"
"CSIUtilities"
"DSYApplicationMainArch"
"DSYSysCnxExit"
"DSYSysDlg"
"DSYSysIRMSysAdapter"
"DSYSysProgressHandler"
"DSYSysWebService"
"DSYSysWMIDriver"
"EKCrypto"
"EKPrivateArchive"
"EKSSL"
"ExperienceKernel"
"explicitB-D"
"explicitB"
"explicitU-D"
"explicitU-D_static"
"explicitU"
"explicitU_static"
"HTTPArch"
"InstArch"
"JS0BASEILB"
"JS0CRYPTEXIT"
"JS0DLK"
"JS0FM"
"JS0GROUP"
"JS0PCC"
"JS0SMT"
"jsmn"
"mkl_core_dll"
"mkl_intel_lp64_dll"
"mkl_intel_thread_dll"
"mkl_rt"
"mkl_sequential_dll"
"msmpi"
"SecurityContext"
"SMAAbuCodeGen"
"SMAAspCodeGen"
"SMABasCodeGen"
"SMAFeaBackbone"
"SMAShaCodeGen_DP"
"SMAShaCodeGen_SP"
"SMASimCodeGen"
"standardB"
"standardU"
"standardU_static"
"StringUtilities"
"SysSqlite"
"SystemTSUUID"
"SystemUUID"
)
set(AbqSDK_PRIVATE_INCLUDE_DIRS "${AbqSDK_PRIVATE_INCLUDE_DIR}")
list(APPEND AbqSDK_PRIVATE_INCLUDE_DIRS "${AbqSDK_PRIVATE_INCLUDE_DIR}/win_b64/code/include")

set(FOUND_ABQ_LIB_FULL_PATH)
set(ABQ_LIB_TARGETS)
foreach(LIB ${ABQ_LIB_LIST})
  set(temp${LIB})
  find_library(temp${LIB}
    NAMES
      "${LIB}.lib"
  )
  mark_as_advanced(temp${LIB})
  list(APPEND FOUND_ABQ_LIB_FULL_PATH ${temp${LIB}})

  if (NOT TARGET AbqSDK::${LIB})
    add_library(AbqSDK::${LIB} UNKNOWN IMPORTED)
    set_target_properties(AbqSDK::${LIB}
      PROPERTIES
        IMPORTED_LOCATION ${temp${LIB}} 
        INTERFACE_INCLUDE_DIRECTORIES "${AbqSDK_PRIVATE_INCLUDE_DIRS}")
    list(APPEND ABQ_LIB_TARGETS AbqSDK::${LIB})
  endif ()
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AbqSDK
  REQUIRED_VARS 
    FOUND_ABQ_LIB_FULL_PATH
    AbqSDK_PRIVATE_INCLUDE_DIR)

