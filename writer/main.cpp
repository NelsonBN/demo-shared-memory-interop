#include <iostream>
#include <windows.h> // HANDLE

#ifdef __GNUG__
#include <cxxabi.h>
#include <cstdlib>
#endif

// Função para demangle do nome do tipo
std::string demangle(const char* mangledName) {
#ifdef __GNUG__
    int status = 0;
    char* realName = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);
    std::string result((status == 0) ? realName : mangledName);
    std::free(realName);
    return result;
#else
    // Em outros compiladores, pode retornar o nome original
    return mangledName;
#endif
}


using namespace std;

#pragma pack(push, 4) // Align the struct to 4 bytes
struct NestedValues { // 12 bytes
    int NestedValueInt; // 4 bytes
    float NestedValueFloat; // 4 bytes
    bool NestedValueBool; // 1 byte -> 4 bytes (+3 bytes padding)
};

struct RootValues {
    bool ValueBool1; // 1 byte -> 4 bytes (+3 bytes padding)
    bool ValueBool2; // 1 byte -> 4 bytes (+3 bytes padding)

    int ValueInt1; // 4 bytes
    int ValueInt2; // 4 bytes

    long ValueLong1; // 4 bytes
    long ValueLong2; // 4 bytes

    float ValueFloat1; // 4 bytes
    float ValueFloat2; // 4 bytes

    double ValueDouble1; // 8 bytes
    double ValueDouble2; // 8 bytes

    char ValueChar1; // 1 byte -> 4 bytes (+3 bytes padding)
    char ValueChar2; // 1 byte -> 4 bytes (+3 bytes padding)

    // char ValueChars1[128]; // 128 bytes
    // char ValueChars2[256]; // 256 bytes

    int TotalNested;
    NestedValues Nested[3];
};
#pragma pack(pop) // Reset the struct alignment


int main()
{
    const size_t structSize = sizeof(RootValues);

    // Create the memory-mapped file for car data
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD)structSize, "SharedMemory");

    // Open the map view of the car data
    LPVOID memmoryBuffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    memset(memmoryBuffer, '\0', structSize); // Clear the data on the memory space just to be sure

    cout << "c++" << endl;
    cout << "##########################" << endl << endl;

    cout << "byte: " << sizeof(std::byte) << endl;
    cout << "bool: " << sizeof(bool) << endl;
    cout << "char: " << sizeof(char) << endl;
    cout << "int: " << sizeof(int) << endl;
    cout << "long: " << sizeof(long) << endl;
    cout << "__int64: " << sizeof(__int64) << endl;
    cout << "unsigned __int64: " << sizeof(unsigned __int64) << endl;
    cout << "float: " << sizeof(float) << endl;
    cout << "double: " << sizeof(double) << endl << endl;

    cout << "RootValues: " << sizeof(RootValues) << endl;
    cout << "NestedValues: " << sizeof(NestedValues) << endl << endl;

    cout << "##########################" << endl << endl;

    RootValues rootValues;

    rootValues.ValueBool1 = true;
    rootValues.ValueBool2 = false;

    rootValues.ValueInt1 = 1234;
    rootValues.ValueInt2 = 5678;

    rootValues.ValueLong1 = 12345678;
    rootValues.ValueLong2 = 87654321;

    rootValues.ValueFloat1 = 1234.5678;
    rootValues.ValueFloat2 = 5678.1234;

    rootValues.ValueDouble1 = 12345678.12345678;
    rootValues.ValueDouble2 = 87654321.87654321;

    rootValues.ValueChar1 = 'A';
    rootValues.ValueChar2 = 'B';

    // strcpy(rootValues.ValueChars1, "Hello, World!");
    // strcpy(rootValues.ValueChars2, "I'm here!!!");


    rootValues.TotalNested = 3;
    for (int i = 0; i < rootValues.TotalNested; i++)
    {
        rootValues.Nested[i].NestedValueInt = 11 * (i + 1);
        rootValues.Nested[i].NestedValueFloat = 11.7 * (i + 1);
        rootValues.Nested[i].NestedValueBool = (i % 2 == 0);
    }

    cout << "[" << demangle(typeid(rootValues.ValueBool1).name()) << "] (" << sizeof(rootValues.ValueBool1) << "): " << rootValues.ValueBool1 << endl;
    cout << "[" << demangle(typeid(rootValues.ValueBool2).name()) << "] (" << sizeof(rootValues.ValueBool2) << "): " << rootValues.ValueBool2 << endl;
    cout << "....." << endl;
    cout << "[" << demangle(typeid(rootValues.ValueInt1).name()) << "] (" << sizeof(rootValues.ValueInt1) << "): " << rootValues.ValueInt1 << endl;
    cout << "[" << demangle(typeid(rootValues.ValueInt2).name()) << "] (" << sizeof(rootValues.ValueInt2) << "): " << rootValues.ValueInt2 << endl;
    cout << "....." << endl;
    cout << "[" << demangle(typeid(rootValues.ValueLong1).name()) << "] (" << sizeof(rootValues.ValueLong1) << "): " << rootValues.ValueLong1 << endl;
    cout << "[" << demangle(typeid(rootValues.ValueLong2).name()) << "] (" << sizeof(rootValues.ValueLong2) << "): " << rootValues.ValueLong2 << endl;
    cout << "....." << endl;
    cout << "[" << demangle(typeid(rootValues.ValueFloat1).name()) << "] (" << sizeof(rootValues.ValueFloat1) << "): " << rootValues.ValueFloat1 << endl;
    cout << "[" << demangle(typeid(rootValues.ValueFloat2).name()) << "] (" << sizeof(rootValues.ValueFloat2) << "): " << rootValues.ValueFloat2 << endl;
    cout << "....." << endl;
    cout << "[" << demangle(typeid(rootValues.ValueDouble1).name()) << "] (" << sizeof(rootValues.ValueDouble1) << "): " << rootValues.ValueDouble1 << endl;
    cout << "[" << demangle(typeid(rootValues.ValueDouble2).name()) << "] (" << sizeof(rootValues.ValueDouble2) << "): " << rootValues.ValueDouble2 << endl;
    cout << "....." << endl;
    cout << "[" << demangle(typeid(rootValues.ValueChar1).name()) << "] (" << sizeof(rootValues.ValueChar1) << "): " << rootValues.ValueChar1 << endl;
    cout << "[" << demangle(typeid(rootValues.ValueChar2).name()) << "] (" << sizeof(rootValues.ValueChar2) << "): " << rootValues.ValueChar2 << endl;
    cout << "....." << endl;
    // cout << "[ValueChars1] (" << sizeof(rootValues.ValueChars1) << "): " << rootValues.ValueChars1 << endl;
    // cout << "[ValueChars2] (" << sizeof(rootValues.ValueChars2) << "): " << rootValues.ValueChars2 << endl;
    // cout << "....." << endl;

    for (int i = 0; i < rootValues.TotalNested; i++)
    {
        cout << "\t[" << i << "] --> [" << demangle(typeid(rootValues.Nested[i].NestedValueInt).name()) << "] (" << sizeof(rootValues.Nested[i].NestedValueInt) << "): " << rootValues.Nested[i].NestedValueInt << endl;
        cout << "\t[" << i << "] --> [" << demangle(typeid(rootValues.Nested[i].NestedValueFloat).name()) << "] (" << sizeof(rootValues.Nested[i].NestedValueFloat) << "): " << rootValues.Nested[i].NestedValueFloat << endl;
        cout << "\t[" << i << "] --> [" << demangle(typeid(rootValues.Nested[i].NestedValueBool).name()) << "] (" << sizeof(rootValues.Nested[i].NestedValueBool) << "): " << rootValues.Nested[i].NestedValueBool << endl;
        cout << "....." << endl;
    }

    memcpy((PVOID)memmoryBuffer, &rootValues, structSize);

    cin.get();

    UnmapViewOfFile(memmoryBuffer);
    CloseHandle(hMapFile);

    return 0;
}
