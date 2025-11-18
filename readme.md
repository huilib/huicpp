# Huicpp: Huilib Cplusplus Library

## What is ```Huicpp```?

Huicpp is a library of C++17 components designed with practicality and efficienty in mind.

## Src Directory:
### adaptation:
different function for different platform.

### detail:
detail function for adaptation.

### inl:
inline cpp headers.

### internal:
internal huicpp cpp files.



## Test make:
```
time g++ -g test.cc huicpp.cpp hstr.cpp hexception.cpp hmutex.cpp hlog.cpp -pthread haddr.cpp detail/network_detail.cpp   hsocket.cpp hmstr.cpp hfilestr.cpp hnormalconf.cpp  hjsonconf.cpp -I/home/joseph/deps/nlohmann/include adaptation/ada_json.cpp hconf.cpp hfilelock.cpp hfilehandle.cpp happlock.cpp detail/filesystem_detail.cpp happ.cpp hfilename.cpp hconfigbase.cpp hsharememory.cpp hmessagequeue.cpp hsemaphore.cpp hutil.cpp hprocess.cpp hrandom.cpp
```
```
time g++ -O3 test.cc huicpp.cpp hstr.cpp hexception.cpp hmutex.cpp hlog.cpp -pthread haddr.cpp detail/network_detail.cpp   hsocket.cpp hmstr.cpp hfilestr.cpp hnormalconf.cpp  hjsonconf.cpp -I/home/joseph/deps/nlohmann/include adaptation/ada_json.cpp hconf.cpp hfilelock.cpp hfilehandle.cpp happlock.cpp detail/filesystem_detail.cpp happ.cpp hfilename.cpp hconfigbase.cpp hsharememory.cpp hmessagequeue.cpp hsemaphore.cpp hutil.cpp hprocess.cpp hrandom.cpp
```
