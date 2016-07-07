#include <iostream>
#include <string>
#include <stdlib.h>
#include <unordered_map>
using namespace std;

struct site
{
    int siteName;
    site* newer;
    site* older;
    
    site();
    site(int &inName, site* inNewer, site* inOlder);
    site& operator=(site& rhsObj);
};

site::site()
{
    siteName = -1;
    newer = NULL;
    older = NULL;
}

site::site(int &inName, site* inNewer, site* inOlder)
{
    siteName = inName;
    newer = inNewer;
    older = inOlder;
    
}

site& site::operator=(site &rhsObj)
{
    this->siteName = rhsObj.siteName;
    this->newer = rhsObj.newer;
    this->older = rhsObj.older;
    
    return *this;
}

class cache
{
public:
    unordered_map<int, site> hashMap;
    site* oldest;
    site* newest;
    int cacheSize;
    int currentSize;
    
    cache(int &inCacheSize);
    ~cache();
    
    void update(int &inSite);
    void fillHash();
    void browse();
};

cache::cache(int &inCacheSize)
{
    cacheSize = inCacheSize;
    oldest = newest = NULL;
}

cache::~cache()
{
    ////delete hashMap;
}

void cache::update(int &inSite)
{
   if(hashMap.find(inSite) == hashMap.end())
   {
        site temp(inSite, NULL, NULL);
        hashMap[temp.siteName] = temp;
        
        //no hashMap members yet
        if(newest == NULL)
        {
            oldest = &hashMap[temp.siteName];
            newest = &hashMap[temp.siteName];
            currentSize++;
        }
        //only one hashMap member
        else if (newest == oldest)
        {
            newest->newer = &hashMap[temp.siteName];
            hashMap[temp.siteName].older = newest;
            oldest = newest;
            newest = &hashMap[temp.siteName];
            currentSize++;
            
        }
        //all else
        else
        {
            newest->newer = &hashMap[temp.siteName];
            newest->newer->older = newest;
            newest = &hashMap[temp.siteName];
            currentSize++;
            
            if (currentSize > cacheSize)
            {
            site* deleteMe = oldest;
            oldest = oldest->newer;
            oldest->older = NULL;
            deleteMe->siteName = -1;
            }
            
        }
    }
    else
    {
        newest->newer = &hashMap[inSite];
        hashMap[inSite].older = newest;
        newest = &hashMap[inSite];
        newest->newer = NULL;
    }
}

void cache::fillHash()
{
    for(int ii = 0; ii < cacheSize; ii++)
    {
        int random_site = rand();
        update(random_site);
        cout << "Adding in: " << random_site << endl;
    }
}

int main()
{
    int size = 5;
    cache myCache(size);
    myCache.fillHash();
    
    int browse = rand();
    myCache.update(browse);
    browse = rand();
    myCache.update(browse);
    
    site* temp = myCache.newest;
    while(temp != myCache.oldest)
    {
        cout << temp->siteName << endl;
        temp = temp->older;
    }
    cout << myCache.oldest->siteName << endl;
}