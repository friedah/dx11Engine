#pragma once
#define FAILEDRETURN(result) if(FAILED(result)) return false;
#define FALSERETURN(result) if(!result) return false;
#define ReleasePTR(ptr) if(ptr.get()) ptr.get()->Release();
#define MAXPATHLEN 100

