#include <ntddk.h>

#define DEVICE_NAME L"\\DEVICE\\MyDevice"
#define DRIVER_SYMBOLLINK_NAME L"\\??\\MySymbolLinkName"
#define IO_TEST_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED,FILE_ANY_ACCESS)


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

NTSTATUS createSymbolLink()
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING SymbolLinkName;
    RtlInitUnicodeString(&SymbolLinkName, DRIVER_SYMBOLLINK_NAME);
    UNICODE_STRING deviceName;
    RtlInitUnicodeString(&deviceName, DEVICE_NAME);
    status = IoCreateSymbolicLink(&SymbolLinkName, &deviceName);
    return status;
}


NTSTATUS DriverIRPCtl(PDEVICE_OBJECT obj, PIRP pIrp)
{
    UNREFERENCED_PARAMETER(obj);
    PIO_STACK_LOCATION pIoStackLocation = NULL;
    KdPrint(("entering irp..."));
    // get UMD calls irp
    pIoStackLocation = IoGetCurrentIrpStackLocation(pIrp);
    switch (pIoStackLocation->MajorFunction)
    {
    case IRP_MJ_CREATE://umd open
    {
        KdPrint(("umd call CreateFile()"));
        break;
    }; 
    case IRP_MJ_CLOSE://umd close
    {
        KdPrint(("umd call CloseHandle()"));
        break;
    }
    case IRP_MJ_DEVICE_CONTROL:
    {
        // 获取控制码
        ULONG funcCode = pIoStackLocation->Parameters.DeviceIoControl.IoControlCode;
        if (funcCode == IO_TEST_CODE)
        {
            KdPrint(("JJR：用户层调用了DeviceIoControl 函数"));
            // 取出传入的缓冲区数据和大小
            char* inBuffer = (char*)pIrp->AssociatedIrp.SystemBuffer;
            KdPrint(("JJR：用户层传入数据：%s", inBuffer));
            // inBuffer 既是传入数据的缓冲区，也是传出数据的缓冲区
            strcpy(inBuffer, "驱动层传出数据");
        }
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        // 实际传出缓冲区的大小
        pIrp->IoStatus.Information = pIoStackLocation->Parameters.DeviceIoControl.InputBufferLength;
    }; break;
    case IRP_MJ_WRITE:
    {
        KdPrint(("JJR：用户层调用了WriteFile 函数"));
        char* inBuffer = (char*)pIrp->UserBuffer;
        KdPrint(("JJR：传递的数据>> %s", inBuffer));
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 4;
    }; break;
    case IRP_MJ_READ:
    {
        KdPrint(("JJR：用户层调用了ReadFile 函数"));
        char* inBuffer = (char*)pIrp->UserBuffer;
        KdPrint(("JJR：传递的数据>> %s", inBuffer));
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 4;
    }
    default:
        break;
    }
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 4;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    KdPrint(("quit irp..."));
    return STATUS_SUCCESS;
}

extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT obj, _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(obj);
    UNREFERENCED_PARAMETER(RegistryPath);

    obj->DriverUnload = SampleUnload;

    // 为IRP 事件指定派遣函数，指定的派遣函数可以为不同的事件不同的函数
    // 当用户层调用CreateFile 函数时产生的IRP 事件
    obj->MajorFunction[IRP_MJ_CREATE] = DriverIRPCtl;
    // 当用户层调用CloseHandle 函数时产生的IRP 事件
    obj->MajorFunction[IRP_MJ_CLOSE] = DriverIRPCtl;
    obj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverIRPCtl;
    obj->MajorFunction[IRP_MJ_WRITE] = DriverIRPCtl;
    obj->MajorFunction[IRP_MJ_READ] = DriverIRPCtl;
    if (createDevice(obj) == STATUS_SUCCESS)
    {
        KdPrint(("Create Device SUCCESS !"));
        if (createSymbolLink() == STATUS_SUCCESS)
        {
            KdPrint(("Create SymbolLink SUCCESS !"));
        }
    }
    KdPrint(("Sample driver initialized successfully\n"));
    return STATUS_SUCCESS;
}