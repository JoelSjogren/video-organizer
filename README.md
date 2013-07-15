## NAME ##
vorg - organize your videos

## DESCRIPTION ##
The purpose of this text-based application is to sort your files: films, series and subtitles. If you, for example, give it the files
 - Some.Show.S01E01*.mp4
 - Some.Show.S01E02*.mp4
 - Some Show - 01x02*.srt
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
		├── CD01.avi
		└── filelist
Note that a file called *filelist* is generated in each directory that contains resulting non-directory files. It contains the original filenames of the files. By using the **-u** flag, a *filelist* can be used to undo a previous operation.

## COMPILE & INSTALL ##
Packages needed:

 - scons
 - libboost-regex-dev
	
To compile, run:

	$ scons

To install, copy the generated executable to a directory in your $PATH. I suggest /usr/local/bin:

	# cp vorg /usr/local/bin

## OPTIONS ##
See *HELP.md*.

## REPORTING BUGS ##
If you are dissatisfied with the results, please attach the appropriate *filelist* in your bugreport.
