## NAME ##
vorg - organize your videos

## SYNOPSIS ##
vorg [OPTIONS] [FILES]

## OPTIONS ##

    -u, --undo
        undo organization. FILES may be a *filelist*

    -o DIRECTORY, --outdir DIRECTORY
        use DIRECTORY as root of all output; default is .
        
    -m, --move
        move FILES (default)
        
    -c, --copy
        copy FILES
        
    -l, --link
        hardlink FILES
        
    -v NUM, --verbosity NUM
        choose how much info to output: (each level includes those above it. interactive output is always visible)
        -2: fatal errors
        -1: errors
        0: warnings (default)
        1: verbose
        2: debug

    -h, --help
        just show this text

    -r, --recursive
        step into directories instead of omitting them

    -p, --part
        do not omit .part files
        
    -b, --build-no
        just print the build number
        
    --clean SIZE
        after organizing, for each folder that was part of the input, remove it recursively if its total size (including children) is less than SIZE bytes. SIZE is a natural number, optionally suffixed by K, M or G (2K = 2 * 1024).
    
    --ask-clean
        ask before cleaning a directory. see --clean

## TIPS & TRICKS ##
To download subtitles, check out [addic7ed-cli](https://github.com/BenoitZugmeyer/addic7ed-cli). Because it supports the *filelist* format that vorg uses to save file information, downloading subtitles is as easy as

    $ addic7ed E19.avi

For syncing your video folders, for example [local hdd] -> [usb hdd], rsync is handy:

    $ rsync -a /home/joel/Video /media/USB-HDD/Video
