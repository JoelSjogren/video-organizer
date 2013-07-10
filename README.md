## NAME ##
video-organizer - organize your videos

## SYNOPSIS ##
video-organizer [OPTIONS] [FILES]

## DESCRIPTION ##
The purpose of this terminal application is to sort your files: films, series and subtitles. If you, for example, give it the files
 - Some.Show.S01E01.*.mp4
 - Some.Show.S01E02.*.mp4
 - Some.Show.S01E02.*.srt
 - Best.Movie.Ever.2010.*.avi
it should move them to give you the following file structure:
.
├── Some Show
│   └── S01
│       ├── E01.mp4
│       ├── E02.mp4
│       ├── E02.srt
│       └── filelist
└── Best Movie Ever
    ├── Best Movie Ever.avi
    └── filelist
Note that a file called *filelist* is generated in each directory that contains resulting non-directory files. It contains their original filenames and enables you to undo organizing by using the **-u** flag.

	-u, --undo
		undo organizing. FILES may be a *filelist*
 
	-o, --outdir=DIRECTORY
		use DIRECTORY as root of all output; default is current
		
	-m, --move
		move FILES (default)
		
	-c, --copy
		copy FILES
		
	-l, --link
		hardlink FILES
		

## REPORTING BUGS ##
If you are dissatisfied with the results, please attach the appropriate *filelist*s in your bugreport.