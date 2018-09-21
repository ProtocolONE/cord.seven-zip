set PackageName=SevenZip/1.0.0@common/stable

@call install.deps.cmd

conan create . %PackageName% -pr conan/msvcprofile
conan create . %PackageName% -pr conan/msvcprofiled

conan upload %PackageName% --all -r=p1

