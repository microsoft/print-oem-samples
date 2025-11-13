function global:mdns { emdns; tlsp "dnscache"; kill -f $global:proc }

# enable mDNS subnet forwarding on domain networks (must reboot afterwards or kill mDNS service)
function global:emdns { if (Get-ItemProperty -Path "HKLM:\SOFTWARE\Policies\Microsoft\Windows NT\DNSClient" -Name "EnableMulticast") { reg delete "HKLM\SOFTWARE\Policies\Microsoft\Windows NT\DNSClient" /f /v EnableMulticast } }

function global:tlsp { $pattern = "*$args*"; $global:proc = $($(tlist -s | where { $_ -like $pattern -and $_ -notlike "*powershell.exe*" }).trim() -split ' ')[0]; Write-Host $proc }
