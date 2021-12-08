

#ifdef _MSC_VER
#define MONITOR_DLL_EXPORT	__declspec(dllexport)
#else
#define MONITOR_DLL_EXPORT
#endif



#ifdef __cplusplus
extern "C" {
#endif

MONITOR_DLL_EXPORT void Dummy_entry_qthttpserver_handler_overall(void)
{
}

#ifdef __cplusplus
}
#endif
