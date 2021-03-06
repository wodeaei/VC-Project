//====- X86InstrMMX.td - Describe the X86 Instruction Set --*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the X86 MMX instruction set, defining the instructions,
// and properties of the instructions which are needed for code generation,
// machine code emission, and analysis.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// MMX Multiclasses
//===----------------------------------------------------------------------===//

let Constraints = "$src1 = $dst" in {
  // MMXI_binop_rm - Simple MMX binary operator.
  multiclass MMXI_binop_rm<bits<8> opc, string OpcodeStr, SDNode OpNode,
                           ValueType OpVT, bit Commutable = 0> {
    def rr : MMXI<opc, MRMSrcReg, (outs VR64:$dst),
                  (ins VR64:$src1, VR64:$src2),
                  !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
                  [(set VR64:$dst, (OpVT (OpNode VR64:$src1, VR64:$src2)))]> {
      let isCommutable = Commutable;
    }
    def rm : MMXI<opc, MRMSrcMem, (outs VR64:$dst),
                  (ins VR64:$src1, i64mem:$src2),
                  !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
                  [(set VR64:$dst, (OpVT (OpNode VR64:$src1,
                                         (bitconvert
                                          (load_mmx addr:$src2)))))]>;
  }

  multiclass MMXI_binop_rm_int<bits<8> opc, string OpcodeStr, Intrinsic IntId,
                               bit Commutable = 0> {
    def rr : MMXI<opc, MRMSrcReg, (outs VR64:$dst),
                 (ins VR64:$src1, VR64:$src2),
                 !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
                 [(set VR64:$dst, (IntId VR64:$src1, VR64:$src2))]> {
      let isCommutable = Commutable;
    }
    def rm : MMXI<opc, MRMSrcMem, (outs VR64:$dst),
                 (ins VR64:$src1, i64mem:$src2),
                 !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
                 [(set VR64:$dst, (IntId VR64:$src1,
                                   (bitconvert (load_mmx addr:$src2))))]>;
  }

  // MMXI_binop_rm_v1i64 - Simple MMX binary operator whose type is v1i64.
  //
  // FIXME: we could eliminate this and use MMXI_binop_rm instead if tblgen knew
  // to collapse (bitconvert VT to VT) into its operand.
  //
  multiclass MMXI_binop_rm_v1i64<bits<8> opc, string OpcodeStr, SDNode OpNode,
                                 bit Commutable = 0> {
    def rr : MMXI<opc, MRMSrcReg, (outs VR64:$dst),
                                  (ins VR64:$src1, VR64:$src2),
                  !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
                  [(set VR64:$dst, (v1i64 (OpNode VR64:$src1, VR64:$src2)))]> {
      let isCommutable = Commutable;
    }
    def rm : MMXI<opc, MRMSrcMem, (outs VR64:$dst),
                                  (ins VR64:$src1, i64mem:$src2),
                  !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
                  [(set VR64:$dst,
                    (OpNode VR64:$src1,(load_mmx addr:$src2)))]>;
  }

  multiclass MMXI_binop_rmi_int<bits<8> opc, bits<8> opc2, Format ImmForm,
                                string OpcodeStr, Intrinsic IntId,
                                Intrinsic IntId2> {
    def rr : MMXI<opc, MRMSrcReg, (outs VR64:$dst),
                                  (ins VR64:$src1, VR64:$src2),
                  !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
                  [(set VR64:$dst, (IntId VR64:$src1, VR64:$src2))]>;
    def rm : MMXI<opc, MRMSrcMem, (outs VR64:$dst),
                                  (ins VR64:$src1, i64mem:$src2),
                  !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
                  [(set VR64:$dst, (IntId VR64:$src1,
                                    (bitconvert (load_mmx addr:$src2))))]>;
    def ri : MMXIi8<opc2, ImmForm, (outs VR64:$dst),
                                   (ins VR64:$src1, i32i8imm:$src2),
                    !strconcat(OpcodeStr, "\t{$src2, $dst|$dst, $src2}"),
           [(set VR64:$dst, (IntId2 VR64:$src1, (i32 imm:$src2)))]>;
  }
}

//===----------------------------------------------------------------------===//
// MMX EMMS & FEMMS Instructions
//===----------------------------------------------------------------------===//

def MMX_EMMS  : MMXI<0x77, RawFrm, (outs), (ins), "emms",
                     [(int_x86_mmx_emms)]>;
def MMX_FEMMS : MMXI<0x0E, RawFrm, (outs), (ins), "femms",
                     [(int_x86_mmx_femms)]>;

//===----------------------------------------------------------------------===//
// MMX Scalar Instructions
//===----------------------------------------------------------------------===//

// Data Transfer Instructions
def MMX_MOVD64rr : MMXI<0x6E, MRMSrcReg, (outs VR64:$dst), (ins GR32:$src),
                        "movd\t{$src, $dst|$dst, $src}",
                        [(set VR64:$dst, 
                         (v2i32 (scalar_to_vector GR32:$src)))]>;
let canFoldAsLoad = 1, isReMaterializable = 1 in
def MMX_MOVD64rm : MMXI<0x6E, MRMSrcMem, (outs VR64:$dst), (ins i32mem:$src),
                        "movd\t{$src, $dst|$dst, $src}",
              [(set VR64:$dst,
               (v2i32 (scalar_to_vector (loadi32 addr:$src))))]>;
let mayStore = 1 in
def MMX_MOVD64mr : MMXI<0x7E, MRMDestMem, (outs), (ins i32mem:$dst, VR64:$src),
                        "movd\t{$src, $dst|$dst, $src}", []>;
def MMX_MOVD64grr : MMXI<0x7E, MRMDestReg, (outs), (ins GR32:$dst, VR64:$src),
                        "movd\t{$src, $dst|$dst, $src}", []>;

let neverHasSideEffects = 1 in
def MMX_MOVD64to64rr : MMXRI<0x6E, MRMSrcReg, (outs VR64:$dst), (ins GR64:$src),
                             "movd\t{$src, $dst|$dst, $src}",
                             []>;

let neverHasSideEffects = 1 in
// These are 64 bit moves, but since the OS X assembler doesn't
// recognize a register-register movq, we write them as
// movd.
def MMX_MOVD64from64rr : MMXRI<0x7E, MRMDestReg,
                               (outs GR64:$dst), (ins VR64:$src),
                               "movd\t{$src, $dst|$dst, $src}", []>;
def MMX_MOVD64rrv164 : MMXRI<0x6E, MRMSrcReg, (outs VR64:$dst), (ins GR64:$src),
                             "movd\t{$src, $dst|$dst, $src}",
                             [(set VR64:$dst,
                              (v1i64 (scalar_to_vector GR64:$src)))]>;

let neverHasSideEffects = 1 in
def MMX_MOVQ64rr : MMXI<0x6F, MRMSrcReg, (outs VR64:$dst), (ins VR64:$src),
                        "movq\t{$src, $dst|$dst, $src}", []>;
let canFoldAsLoad = 1, isReMaterializable = 1 in
def MMX_MOVQ64rm : MMXI<0x6F, MRMSrcMem, (outs VR64:$dst), (ins i64mem:$src),
                        "movq\t{$src, $dst|$dst, $src}",
                        [(set VR64:$dst, (load_mmx addr:$src))]>;
def MMX_MOVQ64mr : MMXI<0x7F, MRMDestMem, (outs), (ins i64mem:$dst, VR64:$src),
                        "movq\t{$src, $dst|$dst, $src}",
                        [(store (v1i64 VR64:$src), addr:$dst)]>;

def MMX_MOVDQ2Qrr : SDIi8<0xD6, MRMSrcReg, (outs VR64:$dst), (ins VR128:$src),
                          "movdq2q\t{$src, $dst|$dst, $src}",
                          [(set VR64:$dst,
                            (v1i64 (bitconvert
                            (i64 (vector_extract (v2i64 VR128:$src),
                                  (iPTR 0))))))]>;

def MMX_MOVQ2DQrr : SSDIi8<0xD6, MRMSrcReg, (outs VR128:$dst), (ins VR64:$src),
                           "movq2dq\t{$src, $dst|$dst, $src}",
          [(set VR128:$dst,
            (movl immAllZerosV,
                  (v2i64 (scalar_to_vector
                              (i64 (bitconvert (v1i64 VR64:$src)))))))]>;

let neverHasSideEffects = 1 in
def MMX_MOVQ2FR64rr: SSDIi8<0xD6, MRMSrcReg, (outs FR64:$dst), (ins VR64:$src),
                           "movq2dq\t{$src, $dst|$dst, $src}", []>;

def MMX_MOVFR642Qrr: SDIi8<0xD6, MRMSrcReg, (outs VR64:$dst), (ins FR64:$src),
                           "movdq2q\t{$src, $dst|$dst, $src}", []>;

def MMX_MOVNTQmr  : MMXI<0xE7, MRMDestMem, (outs), (ins i64mem:$dst, VR64:$src),
                         "movntq\t{$src, $dst|$dst, $src}",
                         [(int_x86_mmx_movnt_dq addr:$dst, VR64:$src)]>;

let AddedComplexity = 15 in
// movd to MMX register zero-extends
def MMX_MOVZDI2PDIrr : MMXI<0x6E, MRMSrcReg, (outs VR64:$dst), (ins GR32:$src),
                             "movd\t{$src, $dst|$dst, $src}",
              [(set VR64:$dst,
                    (v2i32 (X86vzmovl (v2i32 (scalar_to_vector GR32:$src)))))]>;
let AddedComplexity = 20 in
def MMX_MOVZDI2PDIrm : MMXI<0x6E, MRMSrcMem, (outs VR64:$dst),
                           (ins i32mem:$src),
                             "movd\t{$src, $dst|$dst, $src}",
          [(set VR64:$dst,
                (v2i32 (X86vzmovl (v2i32
                                   (scalar_to_vector (loadi32 addr:$src))))))]>;

// Arithmetic Instructions

// -- Addition
defm MMX_PADDB : MMXI_binop_rm<0xFC, "paddb", add, v8i8,  1>;
defm MMX_PADDW : MMXI_binop_rm<0xFD, "paddw", add, v4i16, 1>;
defm MMX_PADDD : MMXI_binop_rm<0xFE, "paddd", add, v2i32, 1>;
defm MMX_PADDQ : MMXI_binop_rm<0xD4, "paddq", add, v1i64, 1>;

defm MMX_PADDSB  : MMXI_binop_rm_int<0xEC, "paddsb" , int_x86_mmx_padds_b, 1>;
defm MMX_PADDSW  : MMXI_binop_rm_int<0xED, "paddsw" , int_x86_mmx_padds_w, 1>;

defm MMX_PADDUSB : MMXI_binop_rm_int<0xDC, "paddusb", int_x86_mmx_paddus_b, 1>;
defm MMX_PADDUSW : MMXI_binop_rm_int<0xDD, "paddusw", int_x86_mmx_paddus_w, 1>;

// -- Subtraction
defm MMX_PSUBB : MMXI_binop_rm<0xF8, "psubb", sub, v8i8>;
defm MMX_PSUBW : MMXI_binop_rm<0xF9, "psubw", sub, v4i16>;
defm MMX_PSUBD : MMXI_binop_rm<0xFA, "psubd", sub, v2i32>;
defm MMX_PSUBQ : MMXI_binop_rm<0xFB, "psubq", sub, v1i64>;

defm MMX_PSUBSB  : MMXI_binop_rm_int<0xE8, "psubsb" , int_x86_mmx_psubs_b>;
defm MMX_PSUBSW  : MMXI_binop_rm_int<0xE9, "psubsw" , int_x86_mmx_psubs_w>;

defm MMX_PSUBUSB : MMXI_binop_rm_int<0xD8, "psubusb", int_x86_mmx_psubus_b>;
defm MMX_PSUBUSW : MMXI_binop_rm_int<0xD9, "psubusw", int_x86_mmx_psubus_w>;

// -- Multiplication
defm MMX_PMULLW  : MMXI_binop_rm<0xD5, "pmullw", mul, v4i16, 1>;

defm MMX_PMULHW  : MMXI_binop_rm_int<0xE5, "pmulhw",  int_x86_mmx_pmulh_w,  1>;
defm MMX_PMULHUW : MMXI_binop_rm_int<0xE4, "pmulhuw", int_x86_mmx_pmulhu_w, 1>;
defm MMX_PMULUDQ : MMXI_binop_rm_int<0xF4, "pmuludq", int_x86_mmx_pmulu_dq, 1>;

// -- Miscellanea
defm MMX_PMADDWD : MMXI_binop_rm_int<0xF5, "pmaddwd", int_x86_mmx_pmadd_wd, 1>;

defm MMX_PAVGB   : MMXI_binop_rm_int<0xE0, "pavgb", int_x86_mmx_pavg_b, 1>;
defm MMX_PAVGW   : MMXI_binop_rm_int<0xE3, "pavgw", int_x86_mmx_pavg_w, 1>;

defm MMX_PMINUB  : MMXI_binop_rm_int<0xDA, "pminub", int_x86_mmx_pminu_b, 1>;
defm MMX_PMINSW  : MMXI_binop_rm_int<0xEA, "pminsw", int_x86_mmx_pmins_w, 1>;

defm MMX_PMAXUB  : MMXI_binop_rm_int<0xDE, "pmaxub", int_x86_mmx_pmaxu_b, 1>;
defm MMX_PMAXSW  : MMXI_binop_rm_int<0xEE, "pmaxsw", int_x86_mmx_pmaxs_w, 1>;

defm MMX_PSADBW  : MMXI_binop_rm_int<0xF6, "psadbw", int_x86_mmx_psad_bw, 1>;

// Logical Instructions
defm MMX_PAND : MMXI_binop_rm_v1i64<0xDB, "pand", and, 1>;
defm MMX_POR  : MMXI_binop_rm_v1i64<0xEB, "por" , or,  1>;
defm MMX_PXOR : MMXI_binop_rm_v1i64<0xEF, "pxor", xor, 1>;

let Constraints = "$src1 = $dst" in {
  def MMX_PANDNrr : MMXI<0xDF, MRMSrcReg,
                         (outs VR64:$dst), (ins VR64:$src1, VR64:$src2),
                         "pandn\t{$src2, $dst|$dst, $src2}",
                         [(set VR64:$dst, (v1i64 (and (vnot VR64:$src1),
                                                  VR64:$src2)))]>;
  def MMX_PANDNrm : MMXI<0xDF, MRMSrcMem,
                         (outs VR64:$dst), (ins VR64:$src1, i64mem:$src2),
                         "pandn\t{$src2, $dst|$dst, $src2}",
                         [(set VR64:$dst, (v1i64 (and (vnot VR64:$src1),
                                                  (load addr:$src2))))]>;
}

// Shift Instructions
defm MMX_PSRLW : MMXI_binop_rmi_int<0xD1, 0x71, MRM2r, "psrlw",
                                    int_x86_mmx_psrl_w, int_x86_mmx_psrli_w>;
defm MMX_PSRLD : MMXI_binop_rmi_int<0xD2, 0x72, MRM2r, "psrld",
                                    int_x86_mmx_psrl_d, int_x86_mmx_psrli_d>;
defm MMX_PSRLQ : MMXI_binop_rmi_int<0xD3, 0x73, MRM2r, "psrlq",
                                    int_x86_mmx_psrl_q, int_x86_mmx_psrli_q>;

defm MMX_PSLLW : MMXI_binop_rmi_int<0xF1, 0x71, MRM6r, "psllw",
                                    int_x86_mmx_psll_w, int_x86_mmx_pslli_w>;
defm MMX_PSLLD : MMXI_binop_rmi_int<0xF2, 0x72, MRM6r, "pslld",
                                    int_x86_mmx_psll_d, int_x86_mmx_pslli_d>;
defm MMX_PSLLQ : MMXI_binop_rmi_int<0xF3, 0x73, MRM6r, "psllq",
                                    int_x86_mmx_psll_q, int_x86_mmx_pslli_q>;

defm MMX_PSRAW : MMXI_binop_rmi_int<0xE1, 0x71, MRM4r, "psraw",
                                    int_x86_mmx_psra_w, int_x86_mmx_psrai_w>;
defm MMX_PSRAD : MMXI_binop_rmi_int<0xE2, 0x72, MRM4r, "psrad",
                                    int_x86_mmx_psra_d, int_x86_mmx_psrai_d>;

// Shift up / down and insert zero's.
def : Pat<(v1i64 (X86vshl     VR64:$src, (i8 imm:$amt))),
          (MMX_PSLLQri VR64:$src, (GetLo32XForm imm:$amt))>;
def : Pat<(v1i64 (X86vshr     VR64:$src, (i8 imm:$amt))),
          (MMX_PSRLQri VR64:$src, (GetLo32XForm imm:$amt))>;

// Comparison Instructions
defm MMX_PCMPEQB : MMXI_binop_rm_int<0x74, "pcmpeqb", int_x86_mmx_pcmpeq_b>;
defm MMX_PCMPEQW : MMXI_binop_rm_int<0x75, "pcmpeqw", int_x86_mmx_pcmpeq_w>;
defm MMX_PCMPEQD : MMXI_binop_rm_int<0x76, "pcmpeqd", int_x86_mmx_pcmpeq_d>;

defm MMX_PCMPGTB : MMXI_binop_rm_int<0x64, "pcmpgtb", int_x86_mmx_pcmpgt_b>;
defm MMX_PCMPGTW : MMXI_binop_rm_int<0x65, "pcmpgtw", int_x86_mmx_pcmpgt_w>;
defm MMX_PCMPGTD : MMXI_binop_rm_int<0x66, "pcmpgtd", int_x86_mmx_pcmpgt_d>;

// Conversion Instructions

// -- Unpack Instructions
let Constraints = "$src1 = $dst" in {
  // Unpack High Packed Data Instructions
  def MMX_PUNPCKHBWrr : MMXI<0x68, MRMSrcReg,
                             (outs VR64:$dst), (ins VR64:$src1, VR64:$src2),
                             "punpckhbw\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v8i8 (mmx_unpckh VR64:$src1, VR64:$src2)))]>;
  def MMX_PUNPCKHBWrm : MMXI<0x68, MRMSrcMem,
                             (outs VR64:$dst), (ins VR64:$src1, i64mem:$src2),
                             "punpckhbw\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v8i8 (mmx_unpckh VR64:$src1,
                                      (bc_v8i8 (load_mmx addr:$src2)))))]>;

  def MMX_PUNPCKHWDrr : MMXI<0x69, MRMSrcReg,
                             (outs VR64:$dst), (ins VR64:$src1, VR64:$src2),
                             "punpckhwd\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v4i16 (mmx_unpckh VR64:$src1, VR64:$src2)))]>;
  def MMX_PUNPCKHWDrm : MMXI<0x69, MRMSrcMem,
                             (outs VR64:$dst), (ins VR64:$src1, i64mem:$src2),
                             "punpckhwd\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v4i16 (mmx_unpckh VR64:$src1,
                                       (bc_v4i16 (load_mmx addr:$src2)))))]>;

  def MMX_PUNPCKHDQrr : MMXI<0x6A, MRMSrcReg,
                             (outs VR64:$dst), (ins VR64:$src1, VR64:$src2),
                             "punpckhdq\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v2i32 (mmx_unpckh VR64:$src1, VR64:$src2)))]>;
  def MMX_PUNPCKHDQrm : MMXI<0x6A, MRMSrcMem,
                             (outs VR64:$dst), (ins VR64:$src1, i64mem:$src2),
                             "punpckhdq\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v2i32 (mmx_unpckh VR64:$src1,
                                       (bc_v2i32 (load_mmx addr:$src2)))))]>;

  // Unpack Low Packed Data Instructions
  def MMX_PUNPCKLBWrr : MMXI<0x60, MRMSrcReg,
                             (outs VR64:$dst), (ins VR64:$src1, VR64:$src2),
                             "punpcklbw\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v8i8 (mmx_unpckl VR64:$src1, VR64:$src2)))]>;
  def MMX_PUNPCKLBWrm : MMXI<0x60, MRMSrcMem,
                             (outs VR64:$dst), (ins VR64:$src1, i64mem:$src2),
                             "punpcklbw\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v8i8 (mmx_unpckl VR64:$src1,
                                      (bc_v8i8 (load_mmx addr:$src2)))))]>;

  def MMX_PUNPCKLWDrr : MMXI<0x61, MRMSrcReg,
                             (outs VR64:$dst), (ins VR64:$src1, VR64:$src2),
                             "punpcklwd\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v4i16 (mmx_unpckl VR64:$src1, VR64:$src2)))]>;
  def MMX_PUNPCKLWDrm : MMXI<0x61, MRMSrcMem,
                             (outs VR64:$dst), (ins VR64:$src1, i64mem:$src2),
                             "punpcklwd\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v4i16 (mmx_unpckl VR64:$src1,
                                       (bc_v4i16 (load_mmx addr:$src2)))))]>;

  def MMX_PUNPCKLDQrr : MMXI<0x62, MRMSrcReg,
                             (outs VR64:$dst), (ins VR64:$src1, VR64:$src2),
                             "punpckldq\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v2i32 (mmx_unpckl VR64:$src1, VR64:$src2)))]>;
  def MMX_PUNPCKLDQrm : MMXI<0x62, MRMSrcMem,
                             (outs VR64:$dst), (ins VR64:$src1, i64mem:$src2),
                             "punpckldq\t{$src2, $dst|$dst, $src2}",
                             [(set VR64:$dst,
                               (v2i32 (mmx_unpckl VR64:$src1,
                                       (bc_v2i32 (load_mmx addr:$src2)))))]>;
}

// -- Pack Instructions
defm MMX_PACKSSWB : MMXI_binop_rm_int<0x63, "packsswb", int_x86_mmx_packsswb>;
defm MMX_PACKSSDW : MMXI_binop_rm_int<0x6B, "packssdw", int_x86_mmx_packssdw>;
defm MMX_PACKUSWB : MMXI_binop_rm_int<0x67, "packuswb", int_x86_mmx_packuswb>;

// -- Shuffle Instructions
def MMX_PSHUFWri : MMXIi8<0x70, MRMSrcReg,
                          (outs VR64:$dst), (ins VR64:$src1, i8imm:$src2),
                          "pshufw\t{$src2, $src1, $dst|$dst, $src1, $src2}",
                          [(set VR64:$dst,
                            (v4i16 (mmx_pshufw:$src2 VR64:$src1, (undef))))]>;
def MMX_PSHUFWmi : MMXIi8<0x70, MRMSrcMem,
                          (outs VR64:$dst), (ins i64mem:$src1, i8imm:$src2),
                          "pshufw\t{$src2, $src1, $dst|$dst, $src1, $src2}",
                          [(set VR64:$dst,
                            (mmx_pshufw:$src2 (bc_v4i16 (load_mmx addr:$src1)),
                                              (undef)))]>;

// -- Conversion Instructions
let neverHasSideEffects = 1 in {
def MMX_CVTPD2PIrr  : MMX2I<0x2D, MRMSrcReg, (outs VR64:$dst), (ins VR128:$src),
                            "cvtpd2pi\t{$src, $dst|$dst, $src}", []>;
let mayLoad = 1 in
def MMX_CVTPD2PIrm  : MMX2I<0x2D, MRMSrcMem, (outs VR64:$dst),
                            (ins f128mem:$src),
                            "cvtpd2pi\t{$src, $dst|$dst, $src}", []>;

def MMX_CVTPI2PDrr  : MMX2I<0x2A, MRMSrcReg, (outs VR128:$dst), (ins VR64:$src),
                            "cvtpi2pd\t{$src, $dst|$dst, $src}", []>;
let mayLoad = 1 in
def MMX_CVTPI2PDrm  : MMX2I<0x2A, MRMSrcMem, (outs VR128:$dst),
                            (ins i64mem:$src),
                            "cvtpi2pd\t{$src, $dst|$dst, $src}", []>;

def MMX_CVTPI2PSrr  : MMXI<0x2A, MRMSrcReg, (outs VR128:$dst), (ins VR64:$src),
                           "cvtpi2ps\t{$src, $dst|$dst, $src}", []>;
let mayLoad = 1 in
def MMX_CVTPI2PSrm  : MMXI<0x2A, MRMSrcMem, (outs VR128:$dst),
                           (ins i64mem:$src),
                           "cvtpi2ps\t{$src, $dst|$dst, $src}", []>;

def MMX_CVTPS2PIrr  : MMXI<0x2D, MRMSrcReg, (outs VR64:$dst), (ins VR128:$src),
                           "cvtps2pi\t{$src, $dst|$dst, $src}", []>;
let mayLoad = 1 in
def MMX_CVTPS2PIrm  : MMXI<0x2D, MRMSrcMem, (outs VR64:$dst), (ins f64mem:$src),
                           "cvtps2pi\t{$src, $dst|$dst, $src}", []>;

def MMX_CVTTPD2PIrr : MMX2I<0x2C, MRMSrcReg, (outs VR64:$dst), (ins VR128:$src),
                            "cvttpd2pi\t{$src, $dst|$dst, $src}", []>;
let mayLoad = 1 in
def MMX_CVTTPD2PIrm : MMX2I<0x2C, MRMSrcMem, (outs VR64:$dst),
                            (ins f128mem:$src),
                            "cvttpd2pi\t{$src, $dst|$dst, $src}", []>;

def MMX_CVTTPS2PIrr : MMXI<0x2C, MRMSrcReg, (outs VR64:$dst), (ins VR128:$src),
                           "cvttps2pi\t{$src, $dst|$dst, $src}", []>;
let mayLoad = 1 in
def MMX_CVTTPS2PIrm : MMXI<0x2C, MRMSrcMem, (outs VR64:$dst), (ins f64mem:$src),
                           "cvttps2pi\t{$src, $dst|$dst, $src}", []>;
} // end neverHasSideEffects


// Extract / Insert
def MMX_X86pinsrw : SDNode<"X86ISD::MMX_PINSRW",
                    SDTypeProfile<1, 3, [SDTCisVT<0, v4i16>, SDTCisSameAs<0,1>,
                                         SDTCisVT<2, i32>, SDTCisPtrTy<3>]>>;


def MMX_PEXTRWri  : MMXIi8<0xC5, MRMSrcReg,
                           (outs GR32:$dst), (ins VR64:$src1, i16i8imm:$src2),
                           "pextrw\t{$src2, $src1, $dst|$dst, $src1, $src2}",
                           [(set GR32:$dst, (X86pextrw (v4i16 VR64:$src1),
                                             (iPTR imm:$src2)))]>;
let Constraints = "$src1 = $dst" in {
  def MMX_PINSRWrri : MMXIi8<0xC4, MRMSrcReg,
                      (outs VR64:$dst), 
                      (ins VR64:$src1, GR32:$src2,i16i8imm:$src3),
                      "pinsrw\t{$src3, $src2, $dst|$dst, $src2, $src3}",
                      [(set VR64:$dst, (v4i16 (MMX_X86pinsrw (v4i16 VR64:$src1),
                                               GR32:$src2,(iPTR imm:$src3))))]>;
  def MMX_PINSRWrmi : MMXIi8<0xC4, MRMSrcMem,
                     (outs VR64:$dst),
                     (ins VR64:$src1, i16mem:$src2, i16i8imm:$src3),
                     "pinsrw\t{$src3, $src2, $dst|$dst, $src2, $src3}",
                     [(set VR64:$dst,
                       (v4i16 (MMX_X86pinsrw (v4i16 VR64:$src1),
                               (i32 (anyext (loadi16 addr:$src2))),
                               (iPTR imm:$src3))))]>;
}

// MMX to XMM for vector types
def MMX_X86movq2dq : SDNode<"X86ISD::MOVQ2DQ", SDTypeProfile<1, 1,
                            [SDTCisVT<0, v2i64>, SDTCisVT<1, v1i64>]>>;

def : Pat<(v2i64 (MMX_X86movq2dq VR64:$src)),
          (v2i64 (MMX_MOVQ2DQrr VR64:$src))>;

def : Pat<(v2i64 (MMX_X86movq2dq (load_mmx addr:$src))),
          (v2i64 (MOVQI2PQIrm addr:$src))>;

def : Pat<(v2i64 (MMX_X86movq2dq (v1i64 (bitconvert
                            (v2i32 (scalar_to_vector (loadi32 addr:$src))))))),
          (v2i64 (MOVDI2PDIrm addr:$src))>;

// Mask creation
def MMX_PMOVMSKBrr : MMXI<0xD7, MRMSrcReg, (outs GR32:$dst), (ins VR64:$src),
                          "pmovmskb\t{$src, $dst|$dst, $src}",
                          [(set GR32:$dst, (int_x86_mmx_pmovmskb VR64:$src))]>;

// Misc.
let Uses = [EDI] in
def MMX_MASKMOVQ : MMXI<0xF7, MRMSrcReg, (outs), (ins VR64:$src, VR64:$mask),
                        "maskmovq\t{$mask, $src|$src, $mask}",
                        [(int_x86_mmx_maskmovq VR64:$src, VR64:$mask, EDI)]>;
let Uses = [RDI] in
def MMX_MASKMOVQ64: MMXI64<0xF7, MRMSrcReg, (outs), (ins VR64:$src, VR64:$mask),
                           "maskmovq\t{$mask, $src|$src, $mask}",
                           [(int_x86_mmx_maskmovq VR64:$src, VR64:$mask, RDI)]>;

//===----------------------------------------------------------------------===//
// Alias Instructions
//===----------------------------------------------------------------------===//

// Alias instructions that map zero vector to pxor.
let isReMaterializable = 1, isCodeGenOnly = 1 in {
  // FIXME: Change encoding to pseudo.
  def MMX_V_SET0       : MMXI<0xEF, MRMInitReg, (outs VR64:$dst), (ins), "",
                              [(set VR64:$dst, (v2i32 immAllZerosV))]>;
  def MMX_V_SETALLONES : MMXI<0x76, MRMInitReg, (outs VR64:$dst), (ins), "",
                              [(set VR64:$dst, (v2i32 immAllOnesV))]>;
}

let Predicates = [HasMMX] in {
  def : Pat<(v1i64 immAllZerosV), (MMX_V_SET0)>;
  def : Pat<(v4i16 immAllZerosV), (MMX_V_SET0)>;
  def : Pat<(v8i8  immAllZerosV), (MMX_V_SET0)>;
}

//===----------------------------------------------------------------------===//
// Non-Instruction Patterns
//===----------------------------------------------------------------------===//

// Store 64-bit integer vector values.
def : Pat<(store (v8i8  VR64:$src), addr:$dst),
          (MMX_MOVQ64mr addr:$dst, VR64:$src)>;
def : Pat<(store (v4i16 VR64:$src), addr:$dst),
          (MMX_MOVQ64mr addr:$dst, VR64:$src)>;
def : Pat<(store (v2i32 VR64:$src), addr:$dst),
          (MMX_MOVQ64mr addr:$dst, VR64:$src)>;
def : Pat<(store (v1i64 VR64:$src), addr:$dst),
          (MMX_MOVQ64mr addr:$dst, VR64:$src)>;

// Bit convert.
def : Pat<(v8i8  (bitconvert (v1i64 VR64:$src))), (v8i8  VR64:$src)>;
def : Pat<(v8i8  (bitconvert (v2i32 VR64:$src))), (v8i8  VR64:$src)>;
def : Pat<(v8i8  (bitconvert (v4i16 VR64:$src))), (v8i8  VR64:$src)>;
def : Pat<(v4i16 (bitconvert (v1i64 VR64:$src))), (v4i16 VR64:$src)>;
def : Pat<(v4i16 (bitconvert (v2i32 VR64:$src))), (v4i16 VR64:$src)>;
def : Pat<(v4i16 (bitconvert (v8i8  VR64:$src))), (v4i16 VR64:$src)>;
def : Pat<(v2i32 (bitconvert (v1i64 VR64:$src))), (v2i32 VR64:$src)>;
def : Pat<(v2i32 (bitconvert (v4i16 VR64:$src))), (v2i32 VR64:$src)>;
def : Pat<(v2i32 (bitconvert (v8i8  VR64:$src))), (v2i32 VR64:$src)>;
def : Pat<(v1i64 (bitconvert (v2i32 VR64:$src))), (v1i64 VR64:$src)>;
def : Pat<(v1i64 (bitconvert (v4i16 VR64:$src))), (v1i64 VR64:$src)>;
def : Pat<(v1i64 (bitconvert (v8i8  VR64:$src))), (v1i64 VR64:$src)>;

// 64-bit bit convert.
def : Pat<(v1i64 (bitconvert (i64 GR64:$src))),
          (MMX_MOVD64to64rr GR64:$src)>;
def : Pat<(v2i32 (bitconvert (i64 GR64:$src))),
          (MMX_MOVD64to64rr GR64:$src)>;
def : Pat<(v4i16 (bitconvert (i64 GR64:$src))),
          (MMX_MOVD64to64rr GR64:$src)>;
def : Pat<(v8i8  (bitconvert (i64 GR64:$src))),
          (MMX_MOVD64to64rr GR64:$src)>;
def : Pat<(i64 (bitconvert (v1i64 VR64:$src))),
          (MMX_MOVD64from64rr VR64:$src)>;
def : Pat<(i64 (bitconvert (v2i32 VR64:$src))),
          (MMX_MOVD64from64rr VR64:$src)>;
def : Pat<(i64 (bitconvert (v4i16 VR64:$src))),
          (MMX_MOVD64from64rr VR64:$src)>;
def : Pat<(i64  (bitconvert (v8i8 VR64:$src))),
          (MMX_MOVD64from64rr VR64:$src)>;
def : Pat<(f64 (bitconvert (v1i64 VR64:$src))),
          (MMX_MOVQ2FR64rr VR64:$src)>;
def : Pat<(f64 (bitconvert (v2i32 VR64:$src))),
          (MMX_MOVQ2FR64rr VR64:$src)>;
def : Pat<(f64 (bitconvert (v4i16 VR64:$src))),
          (MMX_MOVQ2FR64rr VR64:$src)>;
def : Pat<(f64 (bitconvert (v8i8 VR64:$src))),
          (MMX_MOVQ2FR64rr VR64:$src)>;
def : Pat<(v1i64 (bitconvert (f64 FR64:$src))),
          (MMX_MOVFR642Qrr FR64:$src)>;
def : Pat<(v2i32 (bitconvert (f64 FR64:$src))),
          (MMX_MOVFR642Qrr FR64:$src)>;
def : Pat<(v4i16 (bitconvert (f64 FR64:$src))),
          (MMX_MOVFR642Qrr FR64:$src)>;
def : Pat<(v8i8 (bitconvert (f64 FR64:$src))),
          (MMX_MOVFR642Qrr FR64:$src)>;

let AddedComplexity = 20 in {
  def : Pat<(v2i32 (X86vzmovl (bc_v2i32 (load_mmx addr:$src)))),
            (MMX_MOVZDI2PDIrm addr:$src)>;
}

// Clear top half.
let AddedComplexity = 15 in {
  def : Pat<(v2i32 (X86vzmovl VR64:$src)),
            (MMX_PUNPCKLDQrr VR64:$src, (v2i32 (MMX_V_SET0)))>;
}

// Patterns to perform canonical versions of vector shuffling.
let AddedComplexity = 10 in {
  def : Pat<(v8i8  (mmx_unpckl_undef VR64:$src, (undef))),
            (MMX_PUNPCKLBWrr VR64:$src, VR64:$src)>;
  def : Pat<(v4i16 (mmx_unpckl_undef VR64:$src, (undef))),
            (MMX_PUNPCKLWDrr VR64:$src, VR64:$src)>;
  def : Pat<(v2i32 (mmx_unpckl_undef VR64:$src, (undef))),
            (MMX_PUNPCKLDQrr VR64:$src, VR64:$src)>;
}

let AddedComplexity = 10 in {
  def : Pat<(v8i8  (mmx_unpckh_undef VR64:$src, (undef))),
            (MMX_PUNPCKHBWrr VR64:$src, VR64:$src)>;
  def : Pat<(v4i16 (mmx_unpckh_undef VR64:$src, (undef))),
            (MMX_PUNPCKHWDrr VR64:$src, VR64:$src)>;
  def : Pat<(v2i32 (mmx_unpckh_undef VR64:$src, (undef))),
            (MMX_PUNPCKHDQrr VR64:$src, VR64:$src)>;
}

// Some special case PANDN patterns.
// FIXME: Get rid of these.
def : Pat<(v1i64 (and (xor VR64:$src1, (bc_v1i64 (v2i32 immAllOnesV))),
                  VR64:$src2)),
          (MMX_PANDNrr VR64:$src1, VR64:$src2)>;
def : Pat<(v1i64 (and (xor VR64:$src1, (bc_v1i64 (v2i32 immAllOnesV))),
                  (load addr:$src2))),
          (MMX_PANDNrm VR64:$src1, addr:$src2)>;

// Move MMX to lower 64-bit of XMM
def : Pat<(v2i64 (scalar_to_vector (i64 (bitconvert (v8i8 VR64:$src))))),
          (v2i64 (MMX_MOVQ2DQrr VR64:$src))>;
def : Pat<(v2i64 (scalar_to_vector (i64 (bitconvert (v4i16 VR64:$src))))),
          (v2i64 (MMX_MOVQ2DQrr VR64:$src))>;
def : Pat<(v2i64 (scalar_to_vector (i64 (bitconvert (v2i32 VR64:$src))))),
          (v2i64 (MMX_MOVQ2DQrr VR64:$src))>;
def : Pat<(v2i64 (scalar_to_vector (i64 (bitconvert (v1i64 VR64:$src))))),
          (v2i64 (MMX_MOVQ2DQrr VR64:$src))>;

// Move lower 64-bit of XMM to MMX.
def : Pat<(v2i32 (bitconvert (i64 (vector_extract (v2i64 VR128:$src),
                                                  (iPTR 0))))),
          (v2i32 (MMX_MOVDQ2Qrr VR128:$src))>;
def : Pat<(v4i16 (bitconvert (i64 (vector_extract (v2i64 VR128:$src),
                                                  (iPTR 0))))),
          (v4i16 (MMX_MOVDQ2Qrr VR128:$src))>;
def : Pat<(v8i8 (bitconvert (i64 (vector_extract (v2i64 VR128:$src),
                                                  (iPTR 0))))),
          (v8i8 (MMX_MOVDQ2Qrr VR128:$src))>;

// Patterns for vector comparisons
def : Pat<(v8i8 (X86pcmpeqb VR64:$src1, VR64:$src2)),
          (MMX_PCMPEQBrr VR64:$src1, VR64:$src2)>;
def : Pat<(v8i8 (X86pcmpeqb VR64:$src1, (bitconvert (load_mmx addr:$src2)))),
          (MMX_PCMPEQBrm VR64:$src1, addr:$src2)>;
def : Pat<(v4i16 (X86pcmpeqw VR64:$src1, VR64:$src2)),
          (MMX_PCMPEQWrr VR64:$src1, VR64:$src2)>;
def : Pat<(v4i16 (X86pcmpeqw VR64:$src1, (bitconvert (load_mmx addr:$src2)))),
          (MMX_PCMPEQWrm VR64:$src1, addr:$src2)>;
def : Pat<(v2i32 (X86pcmpeqd VR64:$src1, VR64:$src2)),
          (MMX_PCMPEQDrr VR64:$src1, VR64:$src2)>;
def : Pat<(v2i32 (X86pcmpeqd VR64:$src1, (bitconvert (load_mmx addr:$src2)))),
          (MMX_PCMPEQDrm VR64:$src1, addr:$src2)>;

def : Pat<(v8i8 (X86pcmpgtb VR64:$src1, VR64:$src2)),
          (MMX_PCMPGTBrr VR64:$src1, VR64:$src2)>;
def : Pat<(v8i8 (X86pcmpgtb VR64:$src1, (bitconvert (load_mmx addr:$src2)))),
          (MMX_PCMPGTBrm VR64:$src1, addr:$src2)>;
def : Pat<(v4i16 (X86pcmpgtw VR64:$src1, VR64:$src2)),
          (MMX_PCMPGTWrr VR64:$src1, VR64:$src2)>;
def : Pat<(v4i16 (X86pcmpgtw VR64:$src1, (bitconvert (load_mmx addr:$src2)))),
          (MMX_PCMPGTWrm VR64:$src1, addr:$src2)>;
def : Pat<(v2i32 (X86pcmpgtd VR64:$src1, VR64:$src2)),
          (MMX_PCMPGTDrr VR64:$src1, VR64:$src2)>;
def : Pat<(v2i32 (X86pcmpgtd VR64:$src1, (bitconvert (load_mmx addr:$src2)))),
          (MMX_PCMPGTDrm VR64:$src1, addr:$src2)>;

// CMOV* - Used to implement the SELECT DAG operation.  Expanded after
// instruction selection into a branch sequence.
let Uses = [EFLAGS], usesCustomInserter = 1 in {
  def CMOV_V1I64 : I<0, Pseudo,
                    (outs VR64:$dst), (ins VR64:$t, VR64:$f, i8imm:$cond),
                    "#CMOV_V1I64 PSEUDO!",
                    [(set VR64:$dst,
                      (v1i64 (X86cmov VR64:$t, VR64:$f, imm:$cond,
                                          EFLAGS)))]>;
}
