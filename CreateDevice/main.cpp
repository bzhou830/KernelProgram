#include <ntddk.h>

#define DEVICE_NAME L"\\DEVICE\\MyDevice"

void DriverUnload(PDRIVER_OBJECT obj)
{
    IoDeleteDevice(obj->DeviceObject);
    KdPrint(("DriverUnload Enter!"));
}

// create device object for driver
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
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT obj, _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);
    NTSTATUS status = STATUS_SUCCESS;
    KdPrint(("DriverEntry Enter!"));
    obj->DriverUnload = DriverUnload;

    if (createDevice(obj) == STATUS_SUCCESS)
    {
        KdPrint(("Create Device SUCCESS !"));
    }
    return status;
}

