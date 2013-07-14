## NAME ##
video-organizer - organize your videos

## SYNOPSIS ##
video-organizer [OPTIONS] [FILES]

## OPTIONS ##

	-u, --undo	
		undo organizing. FILES may be a *filelist*

	-o DIRECTORY, --outdir=DIRECTORY
		use DIRECTORY as root of all output; default is .
		
	-m, --move
		move FILES (default)
		
	-c, --copy
		copy FILES
		
	-l, --link
		hardlink FILES
		
	-v, --verbose
		output more info than usually

	-h, --help
		just show this text

	-r, --recursive
		step into directories instead of omitting them
		TO BE IMPLEMENTED

	-s, --simulate
		do not write changes to disk




