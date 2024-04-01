/*
 * @Author: lt 1035768203@qq.com
 * @Date: 2024-03-23 10:05:58
 * @LastEditors: lt 1035768203@qq.com
 * @LastEditTime: 2024-03-28 10:49:42
 * @FilePath: \accler\src\cpu\o3\customControl.hh
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CPU_O3_CUSCON_HH__
#define __CPU_O3_CUSCON_HH__
#include "cpu/o3/comm.hh"
#include "cpu/o3/dyn_inst_ptr.hh"

namespace gem5
{

namespace o3
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
        int* getInfo(const DynInstPtr &inst);
        int  numOfIdx(int* info);
        bool ckVal(RegIndex idx,int val);
        void setVal(RegIndex idx,int val);
        bool instNotBusy(int instNum,int ldVecNum);
        void setBusyVec(int instNum,int ldVecNum,bool setStatus);
        bool ckInfo(int* info);
        bool checkCanIss(const DynInstPtr &inst);
        void doneInsts(const DynInstPtr &completed_inst);
        std::list<DynInstPtr> notRdyInstList[MaxThreads];
};

} // namespace o3
} // namespace gem5

#endif // __CPU_O3_CUSCON_HH__
