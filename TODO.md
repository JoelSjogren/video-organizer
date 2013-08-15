## TODO ##
The things listed here are by no means things I plan to do. They're a collection of ideas: a collection that will help me whenever I don't know what to do next. Therefore, feel free to add suggestions.
 - switch to using boost for all file operations since we use it for some
 - write a gui
 - fix minor bug when combining --simulate and --clean: no record is kept about which files are removed; hence clean sees files that it shouldn't
 - shallow sort, i.e. don't create directories
 - naming convention: community.s01e01.pilot.dvdrip.xvid-clerks.avi -> Community (uppercase 'C')
 - title recognition: manual (how? argument may be inappropriate for multiple files)
 - improve algorithm for detecting worthless directories: use tree instead of list
 - deprecate --simulate in favor of a new option --ask. -s requires many assumptions of the behavior of the filesystem whereas interactively asking (-a) the user does not.
