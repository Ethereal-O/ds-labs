#define INIT_LENGTH 50000
struct hashnode
{
    bool valid = false;
    int key;
    int val;
};
class cuckoo_hash
{
private:
    hashnode *hash1,*hash2;
    int size=0;
    int hash1method(int key);
    int hash2method(int key);
    void kickandjudge(int key,int val);
    void resize();
public:
    cuckoo_hash();
    ~cuckoo_hash();
    void Insert(int key,int val);
    hashnode* Lookup(int key);
    bool Delete(int key);
};
