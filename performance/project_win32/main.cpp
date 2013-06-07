#include "pch.h"

#include "Performace.h"
#include "profile.h"

int main()
{
    ProfileInit();
    profileFunction();
    ProfileUnInit();
    return 0;
}
