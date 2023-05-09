((c++-mode
  . ((eval . (cov-mode))
     (flycheck-clang-tidy-build-path . "./")
     (flycheck-clang-tidy-extra-options . ("-extra-arg=-std=gnu++17" "-extra-arg=-I/usr/include/c++/11" "-extra-arg=-I/app/include" "-extra-arg=-I/usr/include/x86_64-linux-gnu/c++/11")))))
