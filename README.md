# packcap - A Packet Capture tool.

### This uses the libpcap library as it's base.


### Follow the steps to install the tool:

**1.** Pull the sourcecode from https://github.com/Rithesh17/Network-Monitoring-Tool.git
**2.** Navigate to the main directory.
**3.** Install libpcap library:
	$ sudo apt-get install libpcap-dev

**4.** Get the privileged(root) shell using this command:
	$ sudo su root

**5.** Navigate into the src/tool_src directory
**6.** Run the build.sh file using this command:
	# chmod u+x build.sh
	#./build.sh

**7.** An executable file will be generated at the end of build.sh . Move that into /bin 
	# mv packcap /bin

**8** ./packcap

And you are good to go!

