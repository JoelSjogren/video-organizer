### help section ###
import subprocess
with open('help.cpp', 'w') as help_cpp:
	comment = "// This file is automatically generated.\n" + \
		"// Do not modify it or your changes will be lost.\n"
	command = ['xxd', '-i', 'README.md']
	proc = subprocess.Popen(command, stdout=subprocess.PIPE)
	array = proc.stdout.read()
	help_cpp.write(comment + array)
	
###
env = Environment()
link = env.Object(['args.cpp', 'help.cpp', 'organizer.cpp',
				   'organizer_test.cpp', 'tinyxml2.cpp',
				   'fileman.cpp', 'test.cpp', 'filelist_test.cpp',
				   'filelist.cpp', 'filename.cpp'])
LIBS = ['boost_regex']
env.Program('video-organizer', ['main.cpp'] + link, LIBS=LIBS)
env.Program('test-suite', ['testsuite.cpp'] + link, LIBS=LIBS)