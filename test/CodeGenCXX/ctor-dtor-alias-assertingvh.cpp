// RUN: %clang_cc1 %s -triple i686-linux -O1 -mconstructor-aliases -emit-llvm -o - | FileCheck %s --check-prefix=CHECK-ITANIUM
// RUN: %clang_cc1 %s -triple i386-pc-win32 -O1 -mconstructor-aliases -emit-llvm -o - | FileCheck %s --check-prefix=CHECK-MSVC

namespace ns {
  struct Base {
    virtual ~Base();
  };

  Base::~Base() { }

  template<typename T>
  struct Template {
    struct NestedDerived : public Base {
      virtual ~NestedDerived() { }
    };
  };
}

namespace ns {
  template class Template<int>;
  // CHECK-ITANIUM: $_ZN2ns8TemplateIiE13NestedDerivedD5Ev = comdat any
  // CHECK-MSVC: @"\01??1NestedDerived@?$Template@H@ns@@UAE@XZ" = weak_odr alias bitcast (void (%"struct.ns::Base"*)* @"\01??1Base@ns@@UAE@XZ" to void (%"struct.ns::Template<int>::NestedDerived"*)*)

}
