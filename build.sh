#Run this in terminal
# Command to compile der-an-2018 project
g++ -std=c++11 -g -pthread der-an-2018/main.cpp -o der-an-2018/der der-an-2018/DEROutstation.cpp der-an-2018/DERCommandHandler.cpp der-an-2018/DERCommandHandlerCallbackDefault.cpp der-an-2018/CSVReaderMeasurement.cpp der-an-2018/GenericCurve.cpp der-an-2018/Schedule.cpp -lasiodnp3 -lasiopal -lopendnp3 -lopenpal -Wno-psabi
exit 0