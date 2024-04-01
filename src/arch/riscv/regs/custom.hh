/*
 * @Author: lt 1035768203@qq.com
 * @Date: 2024-03-18 06:53:20
 * @LastEditors: lt 1035768203@qq.com
 * @LastEditTime: 2024-03-18 15:36:57
 * @FilePath: \accler\src\arch\riscv\regs\custom.hh
 * @Description: 
 *      luot_ai implement of a 9 width registers
 *      each has 4 elem of CRegVal 
 * 
 */


#ifndef __ARCH_RISCV_REGS_CUSTOM_HH__
#define __ARCH_RISCV_REGS_CUSTOM_HH__

#include <cstdint>
#include <string>
#include <vector>

#include "arch/generic/vec_pred_reg.hh"
#include "arch/generic/vec_reg.hh"
#include "arch/riscv/types.hh"
#include "base/bitunion.hh"
#include "cpu/reg_class.hh"
#include "debug/CustomRegs.hh"

namespace gem5
{

namespace RiscvISA
{
const int NumCRegs = 9;
const int NumElemInCReg = 4;
const int SizeOfCRegsInBytes = NumElemInCReg * sizeof(CRegElemVal);

using CustomVRegContainer = gem5::VecRegContainer<SizeOfCRegsInBytes>;
using cvreg_t = CustomVRegContainer;



const std::vector<std::string> CVecRegNames = {
    "c0",   "c1",   "c2",   "c3",   "c4",   "c5",   "c6",   "c7",   "c8"
};

inline constexpr RegClass cRegClass =
    RegClass(CRegClass, CRegClassName, NumCRegs, debug::CustomRegs).
        regType<CRegVal>();


} // namespace RiscvISA
} // namespace gem5

#endif // __ARCH_RISCV_REGS_CUSTOM_HH__
