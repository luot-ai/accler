
#ifndef __CPU_MINOR_CUSCON_HH__
#define __CPU_MINOR_CUSCON_HH__
#include "cpu/minor/scoreboard.hh"

#include "cpu/reg_class.hh"
#include "debug/MinorScoreboard.hh"
#include "debug/MinorTiming.hh"

namespace gem5
{

namespace minor
{

class CustomControl
{
    private:    
        std::vector<int> controlVec;
        std::vector<bool> busyVec;
        std::vector<bool> ldBusyVec;
    public:
        CustomControl();
        ~CustomControl();
        void setCtrlVec(int* ctrlV);
        int* getCtrlVec();
        int* getInfo(MinorDynInstPtr inst);
        int  numOfIdx(int* info);
        bool ckVal(RegIndex idx,int val);
        void setVal(RegIndex idx,int val);
        bool ckInfo(int* info);
        bool checkCanIss(MinorDynInstPtr inst);
        void doneInsts(MinorDynInstPtr completed_inst);
};

} // namespace minor
} // namespace gem5

#endif // __CPU_MINOR_CUSCON_HH__
