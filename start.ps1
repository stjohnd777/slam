start.ps1
param (
[Parameter(Mandatory=$true)]
[string]$ImageName
)

# Get UID/GID equivalents for Windows (emulated as 1000/1000)

$USER_ID = 1000
$GROUP_ID = 1000

# Get user profile and working directory

$HOME_DIR = $env:USERPROFILE
$PWD = Get-Location