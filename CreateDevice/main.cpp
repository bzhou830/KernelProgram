#include <ntddk.h>

#define DEVICE_NAME L"\\DEVICE\\MyDevice"

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    KdPrint(("Sample driver Unload called\n"));
}

NTSTATUS createDevice(PDRIVER_OBJECT obj)
{
    UNICODE_STRING deviceName;
    NTSTATUS status = STATUS_SUCCESS;
    RtlInitUnicodeString(&deviceName, DEVICE_NAME);
    DEVICE_OBJECT deviceObj;
    status = IoCreateDevice(
        obj,
        0x4,
        &deviceName,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE, reinterpret_cast<PDEVICE_OBJECT *>(&deviceObj));
    return status;
}

extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = SampleUnload;
    if (createDevice(DriverObject) == STATUS_SUCCESS)
    {
        KdPrint(("Create Device SUCCESS !"));
    }
    KdPrint(("Sample driver initialized successfully\n"));
    return STATUS_SUCCESS;
}