using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;

namespace D2DEZAssetManager
{
    public static class Program
    {
        [STAThread]
        public static void Main(string[] args)
        {
            Bitmap testImage = new Bitmap(@"D:\Important Data\Programming Projects CPP\D2DEZ\D2DEZ\Assets\TestImage.png");
            byte[] testImageBytes = ExportBitmap(testImage);
            testImage.Dispose();
            File.WriteAllBytes(@"D:\Important Data\Programming Projects CPP\D2DEZ\D2DEZ\Assets\TestImage.bin", testImageBytes);
        }
        public static byte[] ExportBitmap(Bitmap source)
        {
            const int bytesPerPixel = 4;
            const int bytesInHeader = 4;
            BitmapData bmpData = source.LockBits(new Rectangle(0, 0, source.Width, source.Height), ImageLockMode.ReadOnly, PixelFormat.Format32bppPArgb);
            byte[] imageData = new byte[(source.Width * source.Height * bytesPerPixel) + bytesInHeader];
            imageData[0] = (byte)source.Width;
            imageData[1] = (byte)(source.Width >> 8);
            imageData[2] = (byte)source.Height;
            imageData[3] = (byte)(source.Height >> 8);
            Marshal.Copy(bmpData.Scan0, imageData, bytesInHeader, imageData.Length - bytesInHeader);
            source.UnlockBits(bmpData);
            return imageData;
        }
    }
}