#include "GlobalProperties.h"
#include "Config.h"


GlobalProperties::GlobalProperties(Config *pconfig):CurlProcess(pconfig){
  m_api__GlobalProperties = NULL;
  m_pconfig = pconfig;
}

/*
  
/// "http://france-life-imaging.fr/api":GlobalProperties is a complexType with complexContent extension of "http://france-life-imaging.fr/api":Object.
class api__GlobalProperties : public api__Object
{ public:
/// Vector of api__IntKeyStringValuePair* with length 0..unbounded
    std::vector<api__IntKeyStringValuePair*> APIErrorCodesAndMessages       0;
/// Vector of std::string with length 1..unbounded
    std::vector<std::string            > supportedTransferProtocol      1;
/// Vector of enum api__Module with length 1..unbounded
    std::vector<enum api__Module       > supportedModule                1;
/// Element email of type xs:string.
    std::string*                         email                          0;	///< Optional element.
/// Element platformDescription of type xs:string.
    std::string*                         platformDescription            0;	///< Optional element.
/// Element minAuthorizedExecutionTimeout of type xs:int.
    int*                                 minAuthorizedExecutionTimeout  0;	///< Optional element.
/// Element maxAuthorizedExecutionTimeout of type xs:int.
    int*                                 maxAuthorizedExecutionTimeout  0;	///< Optional element.
/// Element defaultExecutionTimeout of type xs:int.
    int*                                 defaultExecutionTimeout        0;	///< Optional element.
/// Element isKillExecutionSupported of type xs:boolean.
    bool                                 isKillExecutionSupported       1;	///< Required element.
/// Element defaultStudy of type xs:string.
    std::string*                         defaultStudy                   0;	///< Optional element.
/// Element supportedAPIVersion of type xs:string.
    std::string                          supportedAPIVersion            1;	///< Required element.
};

enum api__Module
{
	api__Module__Processing,	///< xs:string value="Processing"
	api__Module__Data,	///< xs:string value="Data"
	api__Module__Management,	///< xs:string value="Management"
	api__Module__Commercial,	///< xs:string value="Commercial"
};


*/


bool GlobalProperties::request(struct soap *soap){
    if(!m_api__GlobalProperties)
    {
      m_api__GlobalProperties = soap_new_api__GlobalProperties(soap, 1);
    }

    char routeBuf[m_pconfig->URL_MAX_LEN];
    snprintf(routeBuf, sizeof(routeBuf), "%s/platform",
	    m_pconfig->CATIWEB_WEBSERVICE_API.c_str());

    if (m_pconfig->VERBOSE == 1L)
    {
      std::cout << "pipelie route:" << routeBuf << std::endl;
    }

    if(!CurlProcess::request(soap, routeBuf))
    {
      return false;
    }
    
    if (m_pconfig->VERBOSE == 1L)
    {
      std::cout << this->m_resBuf << std::endl;
    }
    
    if(!CurlProcess::parse_reponse_by_json())
    {
      return false;
    }

    rapidjson::Value &val_apiErrorCodesAndMessages = m_document["APIErrorCodesAndMessages"];
    for(int i=0; i< val_apiErrorCodesAndMessages.Size(); i+=1)
    {
      int error_code = val_apiErrorCodesAndMessages[i]["code"].GetInt();
      std::string error_msg = val_apiErrorCodesAndMessages[i]["message"].GetString();

      api__IntKeyStringValuePair * api__IntKeyStringValuePair_ = soap_new_api__IntKeyStringValuePair(soap, 1);
      api__IntKeyStringValuePair_->name = error_code;
      api__IntKeyStringValuePair_->value = error_msg;
      m_api__GlobalProperties->APIErrorCodesAndMessages.push_back(api__IntKeyStringValuePair_);
    }

    rapidjson::Value &val_supportedTransferProtocols = m_document["supportedTransferProtocols"];
    for(int i=0; i< val_supportedTransferProtocols.Size(); i+=1)
    {
      std::string file_protocol = val_supportedTransferProtocols[i].GetString();
      m_api__GlobalProperties->supportedTransferProtocol.push_back(file_protocol);
    }

    rapidjson::Value &val_supportedModules = m_document["supportedModules"];
    for(int i=0; i< val_supportedModules.Size(); i+=1)
    {
      std::string supported_modules = val_supportedModules[i].GetString();
      if(supported_modules=="Processing")
      {
	m_api__GlobalProperties->supportedModule.push_back(api__Module__Processing);
      }else if(supported_modules=="Data")
      {
	m_api__GlobalProperties->supportedModule.push_back(api__Module__Data);
      }else if(supported_modules=="Management")
      {
	m_api__GlobalProperties->supportedModule.push_back(api__Module__Management);
      }else if(supported_modules=="Commercial")
      {
	m_api__GlobalProperties->supportedModule.push_back(api__Module__Commercial);
      }
    }

    m_api__GlobalProperties->email = soap_new_std__string(soap, 1);
    (*m_api__GlobalProperties->email) = m_document["email"].GetString();

    m_api__GlobalProperties->platformDescription = soap_new_std__string(soap, 1);
    (*m_api__GlobalProperties->platformDescription) = m_document["platformDescription"].GetString();
    
    m_api__GlobalProperties->minAuthorizedExecutionTimeout = (int*)soap_malloc(soap, sizeof(int));
    (*m_api__GlobalProperties->minAuthorizedExecutionTimeout) = m_document["minAuthorizedExecutionTimeout"].GetInt();
    
    m_api__GlobalProperties->maxAuthorizedExecutionTimeout = (int*)soap_malloc(soap, sizeof(int));
    (*m_api__GlobalProperties->maxAuthorizedExecutionTimeout) = m_document["maxAuthorizedExecutionTimeout"].GetInt();
    
    m_api__GlobalProperties->defaultExecutionTimeout = (int*)soap_malloc(soap, sizeof(int));
    (*m_api__GlobalProperties->defaultExecutionTimeout) = m_document["defaultExecutionTimeout"].GetInt();
    
    m_api__GlobalProperties->isKillExecutionSupported = m_document["isKillExecutionSupported"].GetBool();
    
    m_api__GlobalProperties->defaultStudy = soap_new_std__string(soap, 1);
    (*m_api__GlobalProperties->defaultStudy) = m_document["defaultStudy"].GetString();

    m_api__GlobalProperties->supportedAPIVersion = m_document["supportedAPIVersion"].GetString();

    return true;
}
