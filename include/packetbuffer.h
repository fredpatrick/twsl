
#ifndef TWSL_PACKETBUFFER_HH
#define TWSL_PACKETBUFFER_HH

#include <string>
#include <utility>

#define BFRMAX 100

namespace twsl {
    class PacketBuffer
    {
        public:
            PacketBuffer(const std::string& tag);
            PacketBuffer(int                bfrlen,
                        const char*        bfr);
            ~PacketBuffer();

            void        reset();

            void        strdat(const std::string& sdat);
            std::string strdat();

            void        intdat(int                idat);
            int         intdat();

            void        dbldat(double             ddat);
            double      dbldat();

            void        pairdat(std::pair<int,int> pdat);
            std::pair<int, int> pairdat();

            std::string tag();
            int         bfrlen();
            char*       bfr();

        private:
            char        bfr_[BFRMAX];
            std::string tag_;
            int         bfrlen_;
            int         bfrndx_;
    };

}
#endif
