using System;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using Avalonia.Controls;
using Avalonia.Interactivity;

namespace GabaritoCorretor.Views;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
    }

    public void open_omr(object source, RoutedEventArgs args)
    {
        Program.run_cmd(Program.DirForUserOs("python", "scripts/omr/main.py", "-o outputs -i inputs"));
    }

    private static string CurrentPath()
    {
        var fullPath = AppDomain.CurrentDomain.BaseDirectory;
        return fullPath;
    }

    
}