#include <functional>

// Evaluate IDs in a lazy manner and cache the response

namespace lazy {
  using lazy_id = std::function<bool()>;
}

namespace {
  lazy::lazy_id _default_lazy = [](){ return true; };
}

namespace lazy {

  class LazyId {
   public:
    LazyId(lazy_id id = _default_lazy) : _id{id} {}
    
    bool operator()() const {
      if (not _cached) {
        _result = _id();
        _cached = true;
      }
      return _result;
    }

   private:
    const lazy_id _id;
    mutable bool _result {false};
    mutable bool _cached {false};
  };

  // Hold multiple LazyIds

  class LazyCuts {
   public:
    // For complex object selection
    LazyCuts(lazy_id presel = _default_lazy,
             lazy_id loose = _default_lazy,
             lazy_id medium = _default_lazy,
             lazy_id tight = _default_lazy)
      : presel{presel}, loose{loose}, medium{medium}, tight{tight} {}

    // For simple working points
    LazyCuts (float value, float loose, float medium, float tight)
      : presel{_default_lazy},
        loose{[=] () { return value > loose; }},
        medium{[=] () { return value > medium; }},
        tight{[=] () { return value > tight; }} {}

    const LazyId presel;
    const LazyId loose;
    const LazyId medium;
    const LazyId tight;
  };

  // Evaluated cuts
  struct Evaled {
    Evaled () {}
    Evaled (const LazyCuts& cuts)
    : loose{cuts.loose()}, medium{cuts.medium()}, tight{cuts.tight()} {}

    bool loose {false};
    bool medium {false};
    bool tight {false};
  };
}
