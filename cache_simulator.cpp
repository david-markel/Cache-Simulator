/***********************************************************
* * 
* Name: David Markel *

* * 
***********************************************************/

#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<iomanip>

using namespace std;

void space(int n, int space);

void space(int n, int space)
{//print spaces - n digits # of spaces
    //counts number of digits in number
    int i = 10;
    int digits = 1;
    while (n / i != 0)
    {
        i = i * 10;
        digits++;
    }

    for (int i = 0; i < space - digits; i++)
    {
        cout<<' ';
        
    }
}

string A2B(string s);

string A2B(string s)
{//converts address to binary
    string binary = "";

    for (int i = s.length() - 1; i >= 0; i--)
    {
        if (s[i] == '1')
            binary = "0001" + binary;
        else if (s[i] == '2')
            binary = "0010" + binary;
        else if (s[i] == '3')
            binary = "0011" + binary;
        else if (s[i] == '4')
            binary = "0100" + binary;
        else if (s[i] == '5')
            binary = "0101" + binary;
        else if (s[i] == '6')
            binary = "0110" + binary;
        else if (s[i] == '7')
            binary = "0111" + binary;
        else if (s[i] == '8')
            binary = "1000" + binary;
        else if (s[i] == '9')
            binary = "1001" + binary;
        else if (s[i] == 'a')
            binary = "1010" + binary;
        else if (s[i] == 'b')
            binary = "1011" + binary;
        else if (s[i] == 'c')
            binary = "1100" + binary;
        else if (s[i] == 'd')
            binary = "1101" + binary;
        else if (s[i] == 'e')
            binary = "1110" + binary;
        else if (s[i] == 'f')
            binary = "1111" + binary;

        else
            binary = binary + "0000";
    }

    return binary;
}

int B2I(string b);

int B2I(string b)
{//converts binary string into integer
    int val = 0, counter = 0;

    for (int i = b.length() - 1; i >= 0; i--)
    {
        val += ((b[i] - '0')  * pow(2, counter));
        counter += 1;
    }

    return val;
}

class Reference
{
    public:
        Reference();
        void SetRef(string str, int s, int a, int l);   //set values of trace ref based on address

        string add;
        int tag_b;
        int index_b;
        int offset_b;

        bool result;

        int tag;
        int index;
        int offset;
};

Reference::Reference()
{//default constructor
    add = "";
    tag_b = index_b = offset_b = 0;
    tag = index = offset = 0;
}

void Reference::SetRef(string str, int s, int a, int l)
{//convert address into tag, index, and offset
    add = str;
    int size = 0;

    while (l != 1)
    {
        l = l / 2;
        offset_b += 1;
    }
    while (s != 1)
    {
        s = s / 2;
        index_b += 1;
    }

    string binary = A2B(str);

    for (int i = 0; i < binary.length(); i++)
        size++;

    tag_b = size - index_b - offset_b;
    //above determines bits dedicated to each value

    int counter, o_counter, i_counter, t_counter;
    counter = o_counter = i_counter = t_counter = 0;

    //converts either bits to values
    for (int i = size - 1; i >= 0; i--)
    {
        if (counter < offset_b)
        {
            offset += ((binary[i] - '0')  * pow(2, o_counter));
            o_counter += 1;
        }

        else if ((counter >= offset_b) && (counter < size - tag_b))
        {
            index += ((binary[i] - '0') * pow(2, i_counter));
            i_counter += 1;
        }

        else
        {
            tag += ((binary[i] - '0') * pow(2, t_counter));
            t_counter += 1;
        }

        counter += 1;
    }
}

class Cache
{//actually stands for a cache line
    public:
        Cache();
        ~Cache();

        int index;
        int assoc;
        int* way;

        void LRU(int mru);
        void SetA(int n);
};

Cache::Cache()
{
    index = 0;
}

Cache::~Cache()
{
    delete [] way;
}

void Cache::SetA(int n)
{
    assoc = n;
    way = new int[assoc];
}

void Cache::LRU(int mru)
{//calculates LRU by putting MRU in back so LRU is always index 0
    int temp = way[mru];

    for (int i = mru; i < assoc; i++) //put L in front and shift right
        way[i] = way[i + 1];
        
    way[assoc - 1] = temp;
}

int main()
{//variables needed

  
    cout<<"Please enter input file as .txt (size < 30), config will be taken from memory.config: ";
    char fname[30];

    cin>> fname;

    string junk, hex;
    string result = "miss";
    fstream file;

    bool noTag = true;
    bool unaligned = false;

    int sets, assoc, Lsize;
    int entries = 0;

    int hits = 0;
    int misses = 0;

    int MRU, LRU = 0;

    double h_ratio = 0.0;
    double m_ratio = 0.0;

    cout.setf(ios::fixed);  //round to two decimal places
    cout.precision(6);

    //read in configuration
    file.open("memory.config", ios::in);

    if (file.is_open())
    {
        getline(file, junk, ':');
        file>>sets;
        file.ignore(1);

        getline(file, junk, ':');
        file>>assoc;
        file.ignore(1);

        getline(file, junk, ':');
        file>>Lsize;
    }

    file.close();

    file.open(fname, ios::in);
    //see how many references there will be
    if (file.is_open())
    {
        while (getline(file, junk))
        {
            entries += 1;
        }
    }

    file.close();
    file.open(fname, ios::in);

    Reference* list = new Reference[entries];

    //goes through again and sets references based on addresses given
    if (file.is_open())
    {
        string temp;
        for (int i = 0; i < entries; i++)
        {
            getline(file, temp);
            list[i].SetRef(temp, sets, assoc, Lsize);
        }
    }

    file.close();

    //alignment check
    for (int i = 0; i < entries; i++)
    {
        int temp = B2I(A2B(list[i].add));

        if (temp % 4 != 0)
            unaligned = true;
    }

    if (unaligned == true)
        cout<<"cacher: misaligned reference"<<endl;

    else
    {
        //determine amount of unique indexes
        int unique = 1;
        int x, y;

        for (x = 1; x < entries; x++)
        {
            for (y = 0; y < x; y++)
            {
                if (list[x].index == list[y].index)
                {
                    break;
                }
            }
            if (x == y)
            {
                unique += 1;
            }
        }
        
        //create list of cache lines based on unique indexes
        Cache* cache = new Cache[unique];
        for (int i = 0; i < unique; i++)
            cache[i].SetA(assoc);

        int current = 0;
        bool stop;

        for (int i = 0; i < entries; i++) //create list of unique indexes
        {
            stop = false;
            for (int j = 0; j < unique; j++)
            {
                if (list[i].index == cache[j].index)
                    stop = true;     //index already in
            }

            if (stop == false)
            {
                cache[current].index = list[i].index;
                current += 1;
            }
        }

        //cache algorithm
        for (int i = 0; i < entries; i++)
        {
            for (int j = 0; j < unique; j++)
            {
                noTag = true;
                if (list[i].index == cache[j].index)        //find the right index
                {
                    for (int z = 0; z < assoc; z++)         //does it match? hit
                    {
                        if (list[i].tag == cache[j].way[z])
                        {
                            list[i].result = true;
                            hits++;
                            MRU = z;
                            noTag = false;
                            break;
                        }
                    }

                    if (noTag == true)                      //does it miss? change tag of LRU
                    {
                        misses++;
                        list[i].result = false;
                        MRU = 0;
                        cache[j].way[LRU] = list[i].tag;
                    }
                    cache[j].LRU(MRU);
                }
            }
        }

        //formatting of data
        cout<<"Cache Configuration\n\n\n\t"<<sets<<" "<<assoc<<"-way set associative entries\n\tof line size "<<Lsize<<" bytes\n\n\nResults for Each Reference\n\n";

        cout<<"Ref  Address\tTag   Index Offset Result\n---- -------- ------- ----- ------ ------\n";

        for (int i = 0; i < entries; i++)
        {//space function provides appropriate alignment
            space(i, 4);
            cout<<i;
            
            for (int j = 0; j < 9 - list[i].add.length(); j++)
                cout<<' ';

            cout<<list[i].add;

            space(list[i].tag, 8);
            cout<<std::hex<<list[i].tag<<std::dec;
            
            space(list[i].index, 6);
            cout<<list[i].index;
            
            space(list[i].offset, 7);
            cout<<list[i].offset;

            if (list[i].result == false)
                result = "miss";
            else
                result = "hit";

            for (int j = 0; j < 7 - result.length(); j++)
                cout<<' ';

            cout<<result<<endl;
        }

        //calculate ratios
        h_ratio = hits / double(entries);
        m_ratio = misses / double(entries);

        cout<<"\nSimulation Summary Statistics\n-----------------------------";
        cout<<"\nTotal hits       : "<<hits;
        cout<<"\nTotal misses     : "<<misses;
        cout<<"\nTotal accesses   : "<<entries;
        cout<<"\nHit ratio        : "<<h_ratio<<"\nMiss ratio       : "<<m_ratio<<endl;

        //memory clean up
        delete [] cache;
        delete [] list;
    }
    return 0;
}