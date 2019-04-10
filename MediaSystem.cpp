/*
 * Copyright 2014 Fraunhofer FOKUS
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cdmi.h"
#include "MediaSession.h"

namespace CDMi {

class ClearKey : public IMediaKeys {
private:
    ClearKey (const ClearKey&) = delete;
    ClearKey& operator= (const ClearKey&) = delete;

public:
    ClearKey() {
    }
    virtual ~ClearKey() {
    }

    virtual CDMi_RESULT CreateMediaKeySession(
        int32_t licenseType,
        const char *f_pwszInitDataType,
        const uint8_t *f_pbInitData,
        uint32_t f_cbInitData,
        const uint8_t *f_pbCDMData,
        uint32_t f_cbCDMData,
        IMediaKeySession **f_ppiMediaKeySession) {

        *f_ppiMediaKeySession = new CDMi::MediaKeySession();

        CDMi_RESULT dr = poMediaKeySession->Init(licenseType,
             f_pwszInitDataType,
             f_pbInitData,
             f_cbInitData,
             f_pbCDMData,
             f_cbCDMData);


        if (dr != CDMi_SUCCESS)
            delete *f_ppiMediaKeySession;

        return dr;
    }

    virtual CDMi_RESULT SetServerCertificate(
        const uint8_t *f_pbServerCertificate,
        uint32_t f_cbServerCertificate) {
        return CDMi_S_FALSE;
    }

    virtual CDMi_RESULT DestroyMediaKeySession(
        IMediaKeySession *f_piMediaKeySession) {

        delete f_piMediaKeySession;

        return CDMi_SUCCESS;
    }
};

static SystemFactoryType<ClearKey> g_instance({"video/x-h264", "audio/mpeg"});

}  // namespace CDMi

CDMi::ISystemFactory* GetSystemFactory() {

    return (&CDMi::g_instance); 
}
