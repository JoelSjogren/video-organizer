## NAME ##
vorg - organize your videos

## SYNOPSIS ##
vorg [OPTIONS] [FILES]

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

	-s, --simulate
		do not write changes to disk

## TIPS & TRICKS ##
When vorg moves files out of directories, rendering them (almost) empty, it doesn't remove the directories. You may want to recursively remove directories smaller than a given size, say 1 MB. To do this, issue:

    $ du -sk [FILES] |
      awk 'BEGIN { FS="\t" }; { if($1 < 1000) printf "%s\0",$2 }' |
      xargs -0 rm -rf

To download subtitles, check out [addic7ed-cli](https://github.com/BenoitZugmeyer/addic7ed-cli). Because it supports the *filelist* format, downloading subtitles is as easy as

    $ addic7ed E19.avi
