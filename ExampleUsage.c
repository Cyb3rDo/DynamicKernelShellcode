// Driver example using the shellcode to resolve function addresses.
#include <ntddk.h>
#pragma warning(disable:4152) // Allow nonstandard function/data pointer conversions

typedef DWORD64 QWORD;

NTSTATUS DriverUnload(PDRIVER_OBJECT DriverObject)
{
  UNREFERENCED_PARAMETER(DriverObject);
  return(STATUS_SUCCESS);
}

unsigned char shellcode[] = {
  0x48, 0x83, 0xEC, 0x20, 0x57, 0x56, 0x55, 0x53, 0x49, 0x89, 0xC9, 0x65,
  0x48, 0x8B, 0x14, 0x25, 0x38, 0x00, 0x00, 0x00, 0x8B, 0x4A, 0x08, 0x48,
  0xC1, 0xE1, 0x10, 0x66, 0x03, 0x4A, 0x06, 0x48, 0xC1, 0xE1, 0x10, 0x66,
  0x03, 0x0A, 0x48, 0x89, 0xCA, 0x48, 0xC1, 0xEA, 0x0C, 0x48, 0xC1, 0xE2,
  0x0C, 0x48, 0x81, 0xEA, 0x00, 0x10, 0x00, 0x00, 0x48, 0x8B, 0x32, 0x66,
  0x81, 0xFE, 0x4D, 0x5A, 0x75, 0xEF, 0x48, 0x89, 0xD1, 0x8B, 0x5A, 0x3C,
  0x48, 0x01, 0xD9, 0x8B, 0x99, 0x88, 0x00, 0x00, 0x00, 0x48, 0x01, 0xD3,
  0x8B, 0x7B, 0x20, 0x48, 0x01, 0xD7, 0x31, 0xED, 0x8B, 0x34, 0xAF, 0x48,
  0x01, 0xD6, 0xFF, 0xC5, 0xB9, 0x05, 0x15, 0x00, 0x00, 0x31, 0xC0, 0xAC,
  0x41, 0x89, 0xC0, 0x41, 0x83, 0xF8, 0x00, 0x74, 0x0E, 0x89, 0xC8, 0xC1,
  0xE0, 0x05, 0x01, 0xC8, 0x44, 0x01, 0xC0, 0x89, 0xC1, 0xEB, 0xE6, 0x4C,
  0x39, 0xC9, 0x75, 0xD4, 0x8B, 0x7B, 0x24, 0x48, 0x01, 0xD7, 0x66, 0x8B,
  0x2C, 0x6F, 0x8B, 0x7B, 0x1C, 0x48, 0x01, 0xD7, 0x8B, 0x7C, 0xAF, 0xFC,
  0x48, 0x01, 0xD7, 0x48, 0x89, 0xF8, 0x5B, 0x5D, 0x5E, 0x5F, 0x48, 0x83,
  0xC4, 0x20, 0xC3
};



NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
  PVOID	pPool;
  UNREFERENCED_PARAMETER(RegistryPath);

  pPool = ExAllocatePool(NonPagedPool, sizeof(shellcode));
  memmove(pPool, &shellcode, sizeof(shellcode));

  QWORD(*PoolFunction)(DWORD) = pPool;
  DbgPrint("Return Value: %I64X\r\n", PoolFunction(0x86158fb1)); // IoCreateFile
  DbgPrint("Return Value: %I64X\r\n", PoolFunction(0x54a5a00e)); // PsTerminateSystemThread
  DbgPrint("Return Value: %I64X\r\n", PoolFunction(0xba869939)); // PsCreateSystemThread
  ExFreePool(pPool);

  DriverObject->DriverUnload = DriverUnload;
  return(STATUS_SUCCESS);
}
