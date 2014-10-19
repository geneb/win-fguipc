This project is a rudimentary FSUIPC interface for the FlightGear flight simulator.
It's based upon the XConnect project: http://xplaneconnect.sourceforge.net/ and uses a ton of
the code from that project.

win-fguipc is the worst sort of hack job - I did it to prove it could be done and then
Real Life intruded and I no longer had the time to work on it.

The tool communicates with FlightGear using the telnet interface and uses SET & GET commands with
specific property paths.

FlightGear should be configured to spawn the telnet server on port 5501.

Data requests will be logged in a file called "fguipc.log" - when an FSUIPC equipped application
asks for data or writes data, that request should get logged in the file.  This is good for
learning about offsets that are being used, but win-fguipc doesn't currently process. Yet. :)

win-fguipc.cpp is where the whole thing kicks off - the WndProc() callback function is where 
the code handles read & write events.

FGRead.cpp is where the hexadecimal offsets from FSUIPC are matched to routines that will pull
data from FlightGear when referenced.  Very few of the "reader" functions have been implemented,
but there's enough there to see how its to be done for the rest of them.

FGReaders.cpp is where the readers referenced by the hex offsets in FGRead.cpp actually live.

FGWrite.cpp is where the code that writes the contents of hex offsets to FlightGear.  It's 
essentially the write version of what FGRead and FGReaders do in combination.

The layout of the read/write code is as it was found from XConnect and I didn't see any reason to 
change it as it worked.  (If it's not broke, there's no reason to fix it until it is!)

Many thanks to Mat Sutcliffe and his outstanding work on the XConnect project and his help in getting
me up to speed on the code.

-gwb 19Oct14