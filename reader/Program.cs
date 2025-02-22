using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using static System.Console;

const string MEMORY_MAP = "SharedMemory";

WriteLine("C#");
WriteLine("##########################");
WriteLine();

WriteLine($"byte {Marshal.SizeOf(typeof(byte))}");
WriteLine($"bool {Marshal.SizeOf(typeof(bool))}");
WriteLine($"char {Marshal.SizeOf(typeof(char))}");
WriteLine($"int {Marshal.SizeOf(typeof(int))}");
WriteLine($"long {Marshal.SizeOf(typeof(long))}");
WriteLine($"Int64 {Marshal.SizeOf(typeof(long))}"); // System.Int64 = long
WriteLine($"UInt64 {Marshal.SizeOf(typeof(ulong))}");
WriteLine($"float {Marshal.SizeOf(typeof(float))}");
WriteLine($"double {Marshal.SizeOf(typeof(double))}");

WriteLine();

WriteLine($"RootValues {Marshal.SizeOf(typeof(RootValues))}");
WriteLine($"NestedValues {Marshal.SizeOf(typeof(NestedValues))}");


var bufferSizeBytes = Marshal.SizeOf(typeof(RootValues));

using var mapped = MemoryMappedFile.OpenExisting(MEMORY_MAP);
using var viewStream = mapped.CreateViewStream(0, bufferSizeBytes, MemoryMappedFileAccess.Read);

using var sharedMemoryStream = new BinaryReader(viewStream);
var sharedMemoryReadBuffer = sharedMemoryStream.ReadBytes(bufferSizeBytes);

var handleBuffer = GCHandle.Alloc(sharedMemoryReadBuffer, GCHandleType.Pinned);
var data = Marshal.PtrToStructure<RootValues>(handleBuffer.AddrOfPinnedObject());
handleBuffer.Free();

sharedMemoryStream.Close();



WriteLine();
WriteLine("##########################");
WriteLine();

WriteLine($"[{data.ValueBool1.GetType().Name}] ({Marshal.SizeOf(data.ValueBool1)}): {data.ValueBool1}");
WriteLine($"[{data.ValueBool2.GetType().Name}] ({Marshal.SizeOf(data.ValueBool2)}): {data.ValueBool2}");
WriteLine(".....");
WriteLine($"[{data.ValueInt1.GetType().Name}] ({Marshal.SizeOf(data.ValueInt1)}): {data.ValueInt1}");
WriteLine($"[{data.ValueInt2.GetType().Name}] ({Marshal.SizeOf(data.ValueInt2)}): {data.ValueInt2}");
WriteLine(".....");
WriteLine($"[{data.ValueLong1.GetType().Name}] ({Marshal.SizeOf(data.ValueLong1)}): {data.ValueLong1}");
WriteLine($"[{data.ValueLong2.GetType().Name}] ({Marshal.SizeOf(data.ValueLong2)}): {data.ValueLong2}");
WriteLine(".....");
WriteLine($"[{data.ValueFloat1.GetType().Name}] ({Marshal.SizeOf(data.ValueFloat1)}): {data.ValueFloat1}");
WriteLine($"[{data.ValueFloat2.GetType().Name}] ({Marshal.SizeOf(data.ValueFloat2)}): {data.ValueFloat2}");
WriteLine(".....");
WriteLine($"[{data.ValueDouble1.GetType().Name}] ({Marshal.SizeOf(data.ValueDouble1)}): {data.ValueDouble1}");
WriteLine($"[{data.ValueDouble2.GetType().Name}] ({Marshal.SizeOf(data.ValueDouble2)}): {data.ValueDouble2}");
WriteLine(".....");
WriteLine($"[{data.ValueChar1.GetType().Name}] ({Marshal.SizeOf(data.ValueChar1)}): {data.ValueChar1}");
WriteLine($"[{data.ValueChar2.GetType().Name}] ({Marshal.SizeOf(data.ValueChar2)}): {data.ValueChar2}");
WriteLine(".....");
// WriteLine($"[{data.ValueChars1.GetType().Name}] ({Marshal.SizeOf(data.ValueChars1)}): {BitConverter.ToString(data.ValueChars1)}");
// WriteLine($"[{data.ValueChars2.GetType().Name}] ({Marshal.SizeOf(data.ValueChars2)}): {BitConverter.ToString(data.ValueChars2)}");
// WriteLine(".....");

for(var i = 0; i < data.TotalNested; i++)
{
    WriteLine($"\t[{i}] --> [{data.Nested[i].NestedValueInt.GetType().Name}] ({Marshal.SizeOf(data.Nested[i].NestedValueInt)}): {data.Nested[i].NestedValueInt}");
    WriteLine($"\t[{i}] --> [{data.Nested[i].NestedValueFloat.GetType().Name}] ({Marshal.SizeOf(data.Nested[i].NestedValueFloat)}): {data.Nested[i].NestedValueFloat}");
    WriteLine($"\t[{i}] --> [{data.Nested[i].NestedValueBool.GetType().Name}] ({Marshal.SizeOf(data.Nested[i].NestedValueBool)}): {data.Nested[i].NestedValueBool}");
    WriteLine(".....");
}

ReadLine();

[StructLayout(LayoutKind.Sequential, Pack = 4, CharSet = CharSet.Ansi)]
public struct NestedValues
{
    public int NestedValueInt; // 4 bytes
    public float NestedValueFloat; // 4 bytes

    [MarshalAs(UnmanagedType.U1)]
    public bool NestedValueBool; // 1 byte -> 4 bytes (+3 bytes padding)

    public override string ToString()
        => $"{nameof(NestedValueInt)}: '{NestedValueInt}', {nameof(NestedValueFloat)}: '{NestedValueFloat}', {nameof(NestedValueBool)}: '{NestedValueBool}'";
}

[StructLayout(LayoutKind.Sequential, Pack = 4, CharSet = CharSet.Ansi)]
public struct RootValues
{
    [MarshalAs(UnmanagedType.U1)]
    public bool ValueBool1; // 1 byte -> 4 bytes (+3 bytes padding)

    [MarshalAs(UnmanagedType.U1)]
    public bool ValueBool2; // 1 byte -> 4 bytes (+3 bytes padding)

    public int ValueInt1; // 4 bytes
    public int ValueInt2; // 4 bytes

    public int ValueLong1; // 4 bytes

    public int ValueLong2; // 4 bytes

    public float ValueFloat1; // 4 bytes
    public float ValueFloat2; // 4 bytes

    public double ValueDouble1; // 8 bytes
    public double ValueDouble2; // 8 bytes

    [MarshalAs(UnmanagedType.U1)]
    public char ValueChar1; // 1 byte -> 4 bytes (+3 bytes padding)

    [MarshalAs(UnmanagedType.U1)]
    public char ValueChar2; // 1 byte -> 4 bytes (+3 bytes padding)


    // [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
    // public byte[] ValueChars1;

    // [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
    // public byte[] ValueChars2;


    public int TotalNested;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
    public NestedValues[] Nested;
};
