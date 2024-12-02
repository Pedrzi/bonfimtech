using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using Avalonia;

namespace GabaritoCorretor;

internal sealed class Program
{
    // Initialization code. Don't use any Avalonia, third-party APIs or any
    // SynchronizationContext-reliant code before AppMain is called: things aren't initialized
    // yet and stuff might break.
    [STAThread]
    public static void Main(string[] args)
    {
        Console.WriteLine("starting GabaritoCorretor");
        try
        {
            InstallDependecies();
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            throw;
        }
        BuildAvaloniaApp()
            .StartWithClassicDesktopLifetime(args);
        
    }

    // Avalonia configuration, don't remove; also used by visual designer.
    private static AppBuilder BuildAvaloniaApp()
    {
        return AppBuilder.Configure<App>()
            .UsePlatformDetect()
            .WithInterFont()
            .LogToTrace();
    }
    
    public static void run_cmd(string[] args)
    {
        var start = new ProcessStartInfo
        {
            FileName = args[0],
            Arguments = args[1] + " " + args[2],
            UseShellExecute = false,
            RedirectStandardOutput = true
        };
        using var process = Process.Start(start);
        using var reader = process.StandardOutput;
        var result = reader.ReadToEnd();
        Console.WriteLine(result);
    }

    private static void InstallDependecies()
    {
        var filePath = string.Join("", DirForUserOs("scripts/omr/requirements.txt"));
        var dependecies = String.Join(" ", File.ReadAllLines(filePath));
        string[] deps = ["pip", "install -r --user", filePath];
        run_cmd(deps);
    }
    
    public static string[] DirForUserOs(params string[] dirs)
    {
        if (dirs == null || dirs.Length == 0) throw new ArgumentNullException(nameof(dirs));
        if (!RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            return dirs;

        return dirs.Select(dirPath => dirPath.Replace('/', '\\')).ToArray();
    }
}