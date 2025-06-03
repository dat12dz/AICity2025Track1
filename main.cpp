#include <stdio.h>
#include "include/msquic/msquic.h"

const QUIC_API_TABLE* MsQuic;
HQUIC Registration;

int main() {
    if (QUIC_FAILED(MsQuicOpen2(&MsQuic))) {
        printf("Fail để open quic.\n");
        return -1;
    }

    QUIC_REGISTRATION_CONFIG RegConfig = {
        "sample",
        QUIC_EXECUTION_PROFILE_LOW_LATENCY
    };

    if (QUIC_FAILED(MsQuic->RegistrationOpen(&RegConfig, &Registration))) {
        printf("Fail để đăng kí quic.\n");
        MsQuicClose(MsQuic);
        return -1;
    }

    printf("Đăng kí quic thành công.\n");

    MsQuic->RegistrationClose(Registration);
    MsQuicClose(MsQuic);

    return 0;
}
