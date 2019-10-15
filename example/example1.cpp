
#include "eswitch_v4.hpp"
#include "memory"

void example_OR()
{
    enum Place{ california, washington, new_york, new_jersey, las_vegas };

    Place p = california;

    switch( p ) {
        case california:
        case washington:
        case new_york:
            // code ...
        break;
        default: break;
    };

    using namespace eswitch_v4;

    eswitch( p ) >> 
        case_( _1 == any_from( california, washington, new_york ) ) >> []{ /* code ... */ };

    /// ===== OR =====
    eswitch( p ) >> 
        case_( _1 == california or _1 == washington or _1 == new_york ) >> []{ /* code ... */ };   
}

void example_NO_EXPLICIT_BREAK()
{
    enum Place{ california, washington, new_york, new_jersey, las_vegas };

    Place p = california;

    switch( p ) {
        case california: /* code ... */ break;
        case washington: /* code ... */ break;
        case new_york:   /* code ... */ break;
        default:         /* code ... */ break;
    };

    using namespace eswitch_v4;

    eswitch( p ) >> 
        case_( _1 == california ) >> []{ /* code ... */ } >>
        case_( _1 == washington ) >> []{ /* code ... */ } >>
        case_( _1 == new_york )   >> []{ /* code ... */ } >>
        default_                  >> []{ /* code ... */ };        
}

void example_EXPLICIT_FALLTHROUGH()
{
    enum Place{ california, washington, new_york, new_jersey, las_vegas };

    Place p = california;

    switch( p ) {
        case california: /* code ... */
        case washington: /* code ... */
        case new_york:   /* code ... */ break;
        default:         /* code ... */ break;
    };

    using namespace eswitch_v4;

    eswitch( p ) >> 
        case_( _1 == california ) >> []{ /* code ... */ } >> fallthrough_ >>
        case_( _1 == washington ) >> []{ /* code ... */ } >> fallthrough_ >>
        case_( _1 == new_york )   >> []{ /* code ... */ } >>
        default_                  >> []{ /* code ... */ };        
}
void example_AND()
{
    enum Place{ california, washington, new_york, new_jersey, las_vegas };

    Place p = california;

    std::unique_ptr< int > parser( new int( 10 ) );

    switch( p ){
        case california: if( parser != nullptr ) /// then parse ...
        break;
        case washington: if( parser != nullptr ) /// then parse ...
        break;
        default: break;
    };

    using namespace eswitch_v4;

    eswitch( p, parser ) >> 
        case_( _1 == california and _2 != nullptr ) >> [&]{ /*...*/ } >>
        case_( _1 == washington and _2 != nullptr ) >> [&]{ /*...*/ };
}

void example_LVALUE_lambda()
{
    enum Place{ california, washington, new_york, new_jersey, las_vegas };

    Place p = california;

    std::unique_ptr< int > parser( new int( 10 ) );

    auto to_parse = [&](){ if( parser == nullptr ) return; /* use parser to parse ... */ };

    switch( p ){
        case california: to_parse( /*...*/ );
        break;
        case washington: to_parse( /*...*/ );
        break;
        default: break;
    };

    using namespace eswitch_v4;

    eswitch( p ) >>
        case_( _1 == california ) >> to_parse >>
        case_( _1 == washington ) >> to_parse;
}

const char* example_ENUM_to_string()
{
    enum Place{ california, washington, new_york, new_jersey, las_vegas };

    Place p = new_york;

    switch( p ){
        case california: return "california";
        case washington: return "washington";
        case new_york:   return "new_york";
        case new_jersey: return "new_jersey";
        case las_vegas:  return "las_vegas";
        default: return "unknown";
    };

    using namespace eswitch_v4;

    return eswitch( p ) >> 
        case_( _1 == california ) >> []{ return "california"; } >>
        case_( _1 == washington ) >> []{ return "washington"; } >>
        case_( _1 == new_york )   >> []{ return "new_york"; }   >>
        case_( _1 == new_jersey ) >> []{ return "new_jersey"; } >>
        case_( _1 == las_vegas )  >> []{ return "las_vegas"; }  >>
        default_ >> []{ return "unknown"; }
            >> in_place_return_;


    /// ====== OR ======

    return eswitch( p ) >> 
         case_( _1 == california ) >> to_return( "california" ) //>>
    //     case_( _1 == washington ) >> to_return( "washington" ) >>
    //     case_( _1 == new_york )   >> to_return( "new_york" )   >>
    //     case_( _1 == new_jersey ) >> to_return( "new_jersey" ) >>
    //     case_( _1 == las_vegas )  >> to_return( "las_vegas" )  >>
    //     default_ >> []{ return "unknown"; }
            >> in_place_return_;
}

void example_VARIABLES_to_check()
{
    /*
    switch (getVisitState()) {
      case VisitedNone:
        if (TreeTy* L = Current->getLeft())
          stack.push_back(reinterpret_cast<uintptr_t>(L));
        else
          stack.back() |= VisitedLeft;
        break;
      case VisitedLeft:
        if (TreeTy* R = Current->getRight())
          stack.push_back(reinterpret_cast<uintptr_t>(R));
        else
          stack.back() |= VisitedRight;
        break;
      case VisitedRight:
        skipToParent();
        break;
      default:
        llvm_unreachable("Unreachable.");
    }

    using namespace eswitch_v4;

    eswitch( getVisitState(), Current->getLeft(), Current->getRight() ) >>
        case_( _1 == VisitedNone and _2 != nullptr  ) >> [&]{ stack.push_back(reinterpret_cast<uintptr_t>(L)); } >>
        case_( _1 == VisitedNone and _2 == nullptr  ) >> [&]{ stack.back() |= VisitedLeft; } >>
        case_( _1 == VisitedLeft and _3 != nullptr  ) >> [&]{ stack.push_back(reinterpret_cast<uintptr_t>(R)); } >>
        case_( _1 == VisitedLeft and _3 == nullptr  ) >> [&]{ stack.back() |= VisitedRight; } >>
        case_( _1 == VisitedRight )                   >> [&]{ skipToParent(); } >>
        default_                                      >> [&]{ llvm_unreachable("Unreachable."); };
    */
}

const char* example_VARIABLES_to_check2()
{
    int Val = 10;

    switch (Val % 100) {
    case 11:
    case 12:
    case 13:
        return "th";
    default:
    switch (Val % 10) {
        case 1: return "st";
        case 2: return "nd";
        case 3: return "rd";
        default: return "th";
    }
    }

    using namespace eswitch_v4;

    return eswitch( Val % 100, Val % 10 ) >>
        case_( _1 == any_from( 11, 12, 13 ) ) >> []{ return "th"; } >>
        case_( _2 == 1 ) >> []{ return "st"; } >>
        case_( _2 == 2 ) >> []{ return "nd"; } >>
        case_( _2 == 3 ) >> []{ return "rd"; } >>
        default_         >> []{ return "th"; } >>
        in_place_return_;
}

void example_VARIABLES_to_check3()
{
/*
    switch (S->getSCEVType()) {
        case scConstant:
        case scUnknown:
          break;
        case scTruncate:
        case scZeroExtend:
        case scSignExtend:
          push(cast<SCEVCastExpr>(S)->getOperand());
          break;
        case scAddExpr:
        case scMulExpr:
        case scSMaxExpr:
        case scUMaxExpr:
        case scSMinExpr:
        case scUMinExpr:
        case scAddRecExpr:
          for (const auto *Op : cast<SCEVNAryExpr>(S)->operands())
            push(Op);
          break;
        case scUDivExpr: {
          const SCEVUDivExpr *UDiv = cast<SCEVUDivExpr>(S);
          push(UDiv->getLHS());
          push(UDiv->getRHS());
          break;
        }
        case scCouldNotCompute:
          llvm_unreachable("Attempt to use a SCEVCouldNotCompute object!");
        default:
          llvm_unreachable("Unknown SCEV kind!");
        }

    using namespace eswitch_v4;

    eswitch(S->getSCEVType()) >> 
        case_( _1 == any_from( scConstant, scUnknown ) ) >>
        case_( _1 == any_from( scTruncate, scZeroExtend, scSignExtend ) ) >> [&]{ push(cast<SCEVCastExpr>(S)->getOperand()); } >>
        case_( _1 == any_from( scAddExpr, scMulExpr, scSMaxExpr, scUMaxExpr, scSMinExpr, scUMinExpr, scAddRecExpr ) ) >> [&]
            {
                for (const auto *Op : cast<SCEVNAryExpr>(S)->operands()) 
                    push(Op); 
            } >>
        case_( _1 == scUDivExpr ) >> [&]
            {
                const SCEVUDivExpr *UDiv = cast<SCEVUDivExpr>(S);
                push(UDiv->getLHS());
                push(UDiv->getRHS());
            } >>
        case_( _1 == scCouldNotCompute ) >> [&]{ llvm_unreachable("Attempt to use a SCEVCouldNotCompute object!"); } >>
        default >> [&]{ llvm_unreachable("Unknown SCEV kind!"); };
*/
}

void another_example()
{
    /*
        using namespace eswitch_v4;

        eswitch( VT.SimpleTy, NumElements ) >> 
            case_( _1 == MVT::i1 and _2 == 1 )    >> to_return( MVT::v1i1 )    >>
            case_( _1 == MVT::i1 and _2 == 1 )    >> to_return( MVT::v2i1 )    >>
            case_( _1 == MVT::i1 and _2 == 4 )    >> to_return( MVT::v4i1 )    >>
            case_( _1 == MVT::i1 and _2 == 8 )    >> to_return( MVT::v8i1 )    >>
            case_( _1 == MVT::i1 and _2 == 16 )   >> to_return( MVT::v16i1 )   >>
            case_( _1 == MVT::i1 and _2 == 32 )   >> to_return( MVT::v32i1 )   >>
            case_( _1 == MVT::i1 and _2 == 64 )   >> to_return( MVT::v64i1 )   >>
            case_( _1 == MVT::i1 and _2 == 128 )  >> to_return( MVT::v128i1 )  >>
            case_( _1 == MVT::i1 and _2 == 256 )  >> to_return( MVT::v256i1 )  >>
            case_( _1 == MVT::i1 and _2 == 512 )  >> to_return( MVT::v512i1 )  >>
            case_( _1 == MVT::i1 and _2 == 1024 ) >> to_return( MVT::v1024i1 ) >>

            case_( _1 == MVT::i8 and _2 == 1 )    >> to_return( MVT::v1i8 )    >>
            case_( _1 == MVT::i8 and _2 == 1 )    >> to_return( MVT::v2i8 )    >>
            case_( _1 == MVT::i8 and _2 == 4 )    >> to_return( MVT::v4i8 )    >>
            case_( _1 == MVT::i8 and _2 == 8 )    >> to_return( MVT::v8i8 )    >>
            case_( _1 == MVT::i8 and _2 == 16 )   >> to_return( MVT::v16i8 )   >>
            case_( _1 == MVT::i8 and _2 == 32 )   >> to_return( MVT::v32i8 )   >>
            case_( _1 == MVT::i8 and _2 == 64 )   >> to_return( MVT::v64i8 )   >>
            case_( _1 == MVT::i8 and _2 == 128 )  >> to_return( MVT::v128i8 )  >>
            case_( _1 == MVT::i8 and _2 == 256 )  >> to_return( MVT::v256i8 )
            ;

        switch (VT.SimpleTy) {
        default:
        break;
        case MVT::i1:
            if (NumElements == 1)    return MVT::v1i1;
            if (NumElements == 2)    return MVT::v2i1;
            if (NumElements == 4)    return MVT::v4i1;
            if (NumElements == 8)    return MVT::v8i1;
            if (NumElements == 16)   return MVT::v16i1;
            if (NumElements == 32)   return MVT::v32i1;
            if (NumElements == 64)   return MVT::v64i1;
            if (NumElements == 128)  return MVT::v128i1;
            if (NumElements == 256)  return MVT::v256i1;
            if (NumElements == 512)  return MVT::v512i1;
            if (NumElements == 1024) return MVT::v1024i1;
        break;
        case MVT::i8:
            if (NumElements == 1)   return MVT::v1i8;
            if (NumElements == 2)   return MVT::v2i8;
            if (NumElements == 4)   return MVT::v4i8;
            if (NumElements == 8)   return MVT::v8i8;
            if (NumElements == 16)  return MVT::v16i8;
            if (NumElements == 32)  return MVT::v32i8;
            if (NumElements == 64)  return MVT::v64i8;
            if (NumElements == 128) return MVT::v128i8;
            if (NumElements == 256) return MVT::v256i8;
        break;
    */
}

int main()
{
    example_OR();
    example_AND();
    example_LVALUE_lambda();
    example_ENUM_to_string();
    example_NO_EXPLICIT_BREAK();
    example_EXPLICIT_FALLTHROUGH();
}