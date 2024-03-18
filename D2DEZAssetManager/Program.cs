using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;

namespace D2DEZAssetManager
{
    public static class Program
    {
        public const string TemplateResourceName = "D2DEZAssetManager.AssetTemplate.h";
        public const string NameMarker = "#NAME#";
        public const string PathMarker = "#PATH#";
        public const string WidthMarker = "#WIDTH#";
        public const string HeightMarker = "#HEIGHT#";
        public const string BufferMarker = "#BUFFER#";
        [STAThread]
        public static void Main(string[] args)
        {
            Import("D:\\Important Data\\Programming Projects CPP\\D2DEZ\\D2DEZ\\Assets\\TestImage.png", "D:\\Important Data\\Programming Projects CPP\\D2DEZ\\D2DEZ\\Assets\\TestImage.h");
        }
        public static void Import(string sourceImageFilePath, string outputHeaderFilePath)
        {
            string name = Path.GetFileNameWithoutExtension(sourceImageFilePath);
            string path = sourceImageFilePath;
            Bitmap bitmap = new Bitmap(sourceImageFilePath);
            string width = bitmap.Width.ToString();
            string height = bitmap.Height.ToString();
            string buffer = BytesToHex(GetPixelData(bitmap));
            bitmap.Dispose();

            Stream stream = typeof(Program).Assembly.GetManifestResourceStream(TemplateResourceName);
            StreamReader reader = new StreamReader(stream);
            string output = reader.ReadToEnd();
            reader.Dispose();
            stream.Dispose();

            output = output.Replace(NameMarker, name);
            output = output.Replace(PathMarker, path);
            output = output.Replace(WidthMarker, width);
            output = output.Replace(HeightMarker, height);
            output = output.Replace(BufferMarker, buffer);

            File.WriteAllText(outputHeaderFilePath, output);
        }
        // Constructs a c++ byte array definition from a c# byte array.
        // In the following form:
        // { 0x10, 0x12, 0xC1 }
        public static string BytesToHex(byte[] data)
        {
            char[] charset = new char[16] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

            char[] output = new char["{ ".Length + (data.Length * "0x00".Length) + ((data.Length - 1) * ", ".Length) + " }".Length];

            int index = 0;

            output[index] = '{'; index++;
            output[index] = ' '; index++;

            output[index] = '0'; index++;
            output[index] = 'x'; index++;
            output[index] = charset[data[0] & 0x0F]; index++;
            output[index] = charset[data[0] >> 4]; index++;

            for (int i = 1; i < data.Length; i++)
            {
                output[index] = ','; index++;
                output[index] = ' '; index++;

                output[index] = '0'; index++;
                output[index] = 'x'; index++;
                output[index] = charset[data[i] >> 4]; index++;
                output[index] = charset[data[i] & 0x0F]; index++;
            }

            output[index] = ' '; index++;
            output[index] = '}';

            return new string(output);
        }
        // Converts any System.Drawing.Bitmap into a byte array.
        // Pixels are stored in the B8G8R8A8_UNORM pixel format.
        public static byte[] GetPixelData(Bitmap source)
        {
            byte[] output = new byte[source.Width * source.Height * 4];

            int offset = 0;
            for (int y = 0; y < source.Height; y++)
            {
                for (int x = 0; x < source.Width; x++)
                {
                    Color pixel = source.GetPixel(x, y);
                    output[offset] = pixel.B; offset++;
                    output[offset] = pixel.G; offset++;
                    output[offset] = pixel.R; offset++;
                    output[offset] = pixel.A; offset++;
                }
            }

            return output;
        }
    }
}