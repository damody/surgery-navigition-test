
dir -r | % {if ($_ -like "*.obj" -or
		$_ -like "*.ilk" -or
		$_ -like "*.ncb" -or
		$_ -like "*.sdf" -or
		$_ -like "*.idb" -or
		$_ -like "*.res" -or
		$_ -like "*.pdb" -or
		$_ -like "*.pch" -or
		$_ -like "*.tlog" -or
		$_ -like "*.lastbuildstate" -or
		$_ -like "*.log" -or
		$_ -like "*.old" -or
		$_ -like "*.bsc" -or
		$_ -like "*.sbr" -or
		$_ -like "*.ipch" -or
		$_ -like "BuildLog.htm")
		{
			"del " + $_.fullname
			del $_.fullname
		}
	}