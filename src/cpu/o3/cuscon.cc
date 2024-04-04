/*
 * @Author: lt 1035768203@qq.com
 * @Date: 2024-03-23 10:39:12
 * @LastEditors: lt 1035768203@qq.com
 * @LastEditTime: 2024-03-28 17:31:49
 * @FilePath: \accler\src\cpu\o3\cuscon.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cpu/o3/cuscon.hh"
#include "cpu/o3/inst_queue.hh"
#include "cpu/o3/dyn_inst.hh"
#include "debug/IQ.hh"
//info索引
#define ISVAL 0
#define DONEVAL 1
#define IST 2
#define IDX1 (IST+1)
#define IDX2 (IST+2)
#define IDX3 (IST+3)
#define TOTALIDX (IDX3+1)
//用指令编号去索引busyVec
//指令编号为0，用VEC编号去索引ldBusyVec
#define VLOAD 0
#define AAMUL02 1
#define AAMUL12 2
#define AAMUL21 3
#define AAMUL31 4
#define AAMUL1221 5
#define TRIADD012 6
#define TRIADD321 7
#define OACC 8
#define VSTORE 9
#define TOTALIST 10
#define TOTALVEC 9
//ld_kernel和其他custom的互定序
#define PRELDK VSTORE
#define STLDK  4 
//VEC状态，指示各个VEC的“进度”
#define EMPTY 0
#define LOADIN 1
#define DONEAAMUL   2
#define DONETRIADD  3
#define OUTVEC  8
#define WBVAL   2

namespace gem5 
{

namespace o3 
{

CustomControl::CustomControl() 
    : controlVec(TOTALVEC, 0), busyVec(TOTALIST, false) ,ldBusyVec(TOTALVEC, false)
{    
    for (int i = 0; i < MaxThreads; ++i) 
    {
        notRdyInstList[i] = std::list<DynInstPtr>();
    } 
}

CustomControl::~CustomControl() 
{
    //这里不需要做任何事情，因为向量是自动管理内存的
}

/*
 * 根据指令的类型，获取指令信息
 *      issueval【指令发射vec阈值】
 *      doneval 【指令完成vec值】
 *      instnum 【指令类型编号】
 *      idx     【指令所需vec索引】   
 * OACC稍微有点说法，因为两组vec的issueval和doneval不太一样，具体在ckinfo和doneist中去处理
 */
int* 
CustomControl::getInfo(const DynInstPtr &inst)
{
    //注意这里是按照vload去初始化的
    int* returnIdx = new int[TOTALIDX];
    returnIdx[ISVAL] = EMPTY;
    returnIdx[DONEVAL] = LOADIN;
    returnIdx[IST] = VLOAD;
    returnIdx[IDX1] = TOTALVEC;
    returnIdx[IDX2] = TOTALVEC;
    returnIdx[IDX3] = TOTALVEC;
    //VLOAD
    if(inst->isVLoad()){
        RegIndex destIdx = inst->cDestIdx();
        returnIdx[IDX1]=destIdx;
        DPRINTF(IQ, "VLOAD: dest vec is %i\n", returnIdx[IDX1]);
    }
    //AAMul
    else if (inst->isAAMul())
    {
        returnIdx[ISVAL]=LOADIN;
        returnIdx[DONEVAL]=DONEAAMUL;
        if (inst->isAAMul02())
        {
            returnIdx[IST]=AAMUL02;
            returnIdx[IDX1]=0;
            returnIdx[IDX2]=2;
            DPRINTF(IQ, "AAMUL02\n");
        }
        else if (inst->isAAMul31())
        {
            returnIdx[IST]=AAMUL31;
            returnIdx[IDX1]=3;
            returnIdx[IDX2]=1;
            DPRINTF(IQ, "AAMUL31\n");
        }    
        else if (inst->isAAMul1221())
        {
            returnIdx[IST]=AAMUL1221;
            returnIdx[IDX1]=1;
            returnIdx[IDX2]=2;
            DPRINTF(IQ, "AAMUL1221\n");
        }
    }
    //TriAdd OACC
    else if (inst->isTriAdd())
    {
        returnIdx[ISVAL]=DONEAAMUL;
        returnIdx[DONEVAL]=DONETRIADD;
        if (inst->isTriAdd012())
        {
            returnIdx[IST]=TRIADD012;
            returnIdx[IDX1]=0;
            returnIdx[IDX2]=1;
            returnIdx[IDX3]=2;
            DPRINTF(IQ, "AAMUL012\n");
        }
        else if (inst->isTriAdd321())
        {
            returnIdx[IST]=TRIADD321;
            returnIdx[IDX1]=3;
            returnIdx[IDX2]=2;
            returnIdx[IDX3]=1;
            DPRINTF(IQ, "AAMUL321\n");
        }
    }
    else if (inst->isOacc())
    {
        returnIdx[ISVAL]=DONETRIADD;
        returnIdx[DONEVAL]=WBVAL;
        returnIdx[IST]=OACC;
        returnIdx[IDX1]=0;
        returnIdx[IDX2]=3;
        returnIdx[IDX3]=OUTVEC;
        DPRINTF(IQ, "OACC\n");
    }
    //VSTORE
    else if (inst->isVStore())
    {
        returnIdx[ISVAL]=WBVAL;
        returnIdx[DONEVAL]=EMPTY;
        returnIdx[IST]=VSTORE;
        returnIdx[IDX1]=OUTVEC;
        DPRINTF(IQ, "VSTORE\n");
    }
    DPRINTF(IQ, "get custom inst infomation: issue val %i,done val %i,idx1 %i,idx2 %i,idx3 %i\n", returnIdx[ISVAL],returnIdx[DONEVAL]
    ,returnIdx[IDX1],returnIdx[IDX2],returnIdx[IDX3]);
    return returnIdx;
}
int
CustomControl::numOfIdx(int* info)
{
    if (info[IDX2]==TOTALVEC) return 1;
    else if(info[IDX3]==TOTALVEC) return 2;
    else return 3;
}

//issue前：检查vec进度
bool 
CustomControl::ckVal(RegIndex idx,int val)
{
    DPRINTF(IQ, "custom check ctrlVec: vec %i ,issue val %i,actual val %i\n", idx,val,controlVec[idx]);
    return (controlVec[idx]==val);
}
//done后：设置vec进度
void
CustomControl::setVal(RegIndex idx,int val)
{
    DPRINTF(IQ, "set custom ctrlVec %i to val %i\n", idx,val);
    controlVec[idx]=val;
}

//防止循环迭代造成的问题，issue前查看该指令是否busy
bool 
CustomControl::instNotBusy(int instNum,int ldVecNum)
{
    bool returnVal;
    if(instNum==VLOAD)
    {   
        returnVal=!ldBusyVec[ldVecNum];
        DPRINTF(IQ, "check if there is a flight ld_tile[%i] inst: %i\n", ldVecNum,!returnVal);
    }
    else
    {
        returnVal=!busyVec[instNum];
        DPRINTF(IQ, "check if there is a flight [%i] inst: %i\n", instNum,!returnVal);
    }
    return returnVal;
}
//issue和done：都需要设置busy位
void 
CustomControl::setBusyVec(int instNum,int ldVecNum,bool setStatus)
{
    if(instNum==VLOAD)
    ldBusyVec[ldVecNum]=setStatus;
    else
    busyVec[instNum]=setStatus;
}

/*
 * 根据指令的类型，检查是否可以发射
 *      是否有同一指令正在操作
 *      ldkernel和其他custom保持互定序
 *      vec的进度：OACC有些特殊
 */
bool 
CustomControl::ckInfo(int* info)
{
    bool returnVal = true;
    //busy
    int ist=info[IST];
    int may_ldvec=info[IDX1];
    if ( !instNotBusy(ist,may_ldvec) ) 
    {
        returnVal = false;
        DPRINTF(IQ, "there is a same custom inst busy\n");
    }
    //load kernel和其他custom指令保持定序
    if ( ist == VLOAD && may_ldvec < OUTVEC && may_ldvec >= STLDK)
    {
        if (!instNotBusy(PRELDK,0))
        {
            returnVal = false;  
            DPRINTF(IQ, "load kernel wait for other custom inst\n");
        }
            
    }
    else
    {
        for ( int ldk = STLDK;ldk < OUTVEC;ldk++)
        {
            if(!instNotBusy(VLOAD,ldk))
            {
                DPRINTF(IQ, "custom load kernel %i inst is being done!\n",ldk);
                returnVal = false;
            }
        }
    }
    //control
    DPRINTF(IQ, "now check ctrlvec: \n");
    for (int i = IDX1; i < IDX1 + numOfIdx(info); i++)
    {
        int isval = info[ISVAL];
        int index = info[i];
        if (index==OUTVEC && ist==OACC) isval = LOADIN;
        if (!ckVal(index,isval))
        {
            returnVal = false;
            break;
        }
    }
    return returnVal;
}
bool
CustomControl::checkCanIss(const DynInstPtr &inst)
{
    assert(inst->isCustom());
    DPRINTF(IQ, "check if ins PC %s custom ready.\n", inst->pcState());
    int* info = getInfo(inst);
    bool canIss = ckInfo(info);
    if(canIss)  
    {
        setBusyVec(info[IST],info[IDX1],true);
        DPRINTF(IQ, "Yes,custom ready\n");
    }    
    delete[] info;
    
    return canIss;
}

/*
 * IEW::writebackInsts调用
 *      操作与iq：wakeupdependents类似
 *          完成busyVec和controlVec的设置
 *          检查notRdyList中是否有指令满足发射条件，若有：
 *              设置can_issue
 *              addifready
 *              从未就绪指令中脱身
 *      这里的dependents包括
 *          数据依赖
 *          控制依赖load&empty
 *          迭代依赖busyVec
 *          互定序依赖
 * vstore其实也不会有事，因为已经完成了数据的复制
 * 访存违例也是不存在，store1->load1->load2
 */
void
CustomControl::doneInsts(const DynInstPtr &completed_inst)
{
    assert(completed_inst->isCustom());
    int* info = getInfo(completed_inst);
    setBusyVec(info[IST],info[IDX1],false);
    if(info[IST]==OACC)
    {
        for (int i=0;i<4;i++) setVal(i,EMPTY);
        setVal(OUTVEC,WBVAL);
    }
    else
    {
        int doneVal = info[DONEVAL];
        setVal(info[IDX1],doneVal);//ld,aamul02,aamul31,triadd,wb
        if(info[IST]==AAMUL1221)
        {setVal(info[IDX2],doneVal);}
    }
    delete[] info;
}


} // namespace o3

} // namespace gem5
