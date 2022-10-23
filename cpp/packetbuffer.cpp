
#include "packetbuffer.h"
#include <string.h>
#include <iostream>

twsl::PacketBuffer::
PacketBuffer(const std::string& tag)
{
    tag_    = tag;
    ::memcpy( bfr_, tag_.c_str(), tag_.length() +1 );
    bfrndx_ = tag_.length() + 1;;
    bfrlen_ = bfrndx_;
}

twsl::PacketBuffer::
PacketBuffer(int                bfrlen,
            const char*        bfr)
{
    bfrlen_ = bfrlen;
    ::memcpy( bfr_, bfr, bfrlen_);
    bfrndx_ = 0;
    tag_    = &bfr_[bfrndx_];
    bfrndx_ += (tag_.length()+1);
}

twsl::PacketBuffer::
~PacketBuffer()
{
}

void
twsl::PacketBuffer::
reset()
{
   bfrndx_ = tag_.length() + 1;
}

std::string
twsl::PacketBuffer::
tag()
{
    return tag_;;
}

int
twsl::PacketBuffer::
bfrlen()
{
    return bfrlen_;
}

char*
twsl::PacketBuffer::
bfr()
{
    return bfr_;
}

void
twsl::PacketBuffer::
strdat(const std::string& sdat)
{
    int ls = sdat.length() + 1;
    ::memcpy(&bfr_[bfrndx_], sdat.c_str(), ls);
    bfrndx_ +=  ls;
    bfrlen_ = bfrndx_;
}

std::string
twsl::PacketBuffer::
strdat()
{
    std::string s = &bfr_[bfrndx_];
    bfrndx_ += (s.length() + 1);
    return s;
}

void 
twsl::PacketBuffer::
intdat(int i)
{
    ::memcpy(&bfr_[bfrndx_], &i, sizeof(int) );
    bfrndx_ += sizeof(int);
    bfrlen_ = bfrndx_;
}

int
twsl::PacketBuffer::
intdat()
{
    int i;
    ::memcpy(&i, &bfr_[bfrndx_], sizeof(int) );
    bfrndx_ += sizeof(int);
    return i;
}
    
void
twsl::PacketBuffer::
dbldat(double d)
{
    ::memcpy(&bfr_[bfrndx_], &d, sizeof(double) );
    bfrndx_ += sizeof(double);
    bfrlen_ = bfrndx_;
}

double
twsl::PacketBuffer::
dbldat()
{
    double d;
    ::memcpy(&d, &bfr_[bfrndx_], sizeof(double) );
    bfrndx_ += sizeof(double);
    return d;
}

void
twsl::PacketBuffer::
pairdat(std::pair<int, int> pdat)
{
    ::memcpy(&bfr_[bfrndx_], &pdat.first  , sizeof(int) );
    bfrndx_ += sizeof(int);
    ::memcpy(&bfr_[bfrndx_], &pdat.second , sizeof(int) );
    bfrndx_ += sizeof(int);
    bfrlen_ = bfrndx_;
}

std::pair<int, int>
twsl::PacketBuffer::
pairdat()
{
    std::pair<int, int> p;
    ::memcpy(&p.first , &bfr_[bfrndx_], sizeof(int));
    bfrndx_ += sizeof(int);
    ::memcpy(&p.second, &bfr_[bfrndx_], sizeof(int));
    bfrndx_ += sizeof(int);
    return p;
}

