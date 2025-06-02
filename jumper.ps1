

function goto {
    param($hook)
    $path = & "C:\Path\To\goto.exe" $hook
    if ($LASTEXITCODE -eq 0) {
        Set-Location $path
    } else {
        Write-Error $path
    }
}