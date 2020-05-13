#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace Neo {
namespace Sidecar {

struct ResponseParam {
  std::string Name;
  std::string Type;
  bool FromBody;
};

struct ResponseApi {
  int Code;
  std::string Message;
  std::string Name;
  std::string ReturnType;
  std::vector<ResponseParam> Params;
  std::vector<std::string> AdditionalEndpoints;
};

struct ResponseService {
  std::string Code;
  std::string Message;
  std::string ServiceName;
  std::string Uri;
  std::vector<ResponseApi> Apis;
  std::vector<std::string> Endpoints;
};

struct ResponseHeader {
  std::string RTime;
  std::string SID;
  std::string SIP;
  std::string SName;
  int StatusCode;
  std::string XCache;
};

struct ResponseServiceGroup {
  std::string Code;
  std::string Message;
  std::string ServiceGroupName;
  std::string Version;
  std::string ContractVersion;
  std::string Platform;
  std::string Tags;
  std::vector<ResponseService> Services;
};

struct ResponseBody {
  std::string Message;
  ResponseServiceGroup ServiceGroups;
  ResponseHeader header;
};
struct V1ContractInfo {

  std::string Cip;
  std::string ServiceGroupName;
  std::string Version;
  std::string string;
  bool Incompatible;
  std::string DefaultEnv;
  std::string SgNaming;
  std::vector<ResponseService> Services;
};

struct V1ContractResponse {
  int Code;
  std::string Msg;
  std::vector<V1ContractInfo> Data;
};

struct RequestHeader {
  /* data */
  std::string CHost;
  std::string CIP;
  std::string CName;
  std::string CVersion;
  std::string ContentType;
  std::string PLevel;
  std::string PVersion;
  std::string RTime;
  std::string SID;
  std::string SName;
  int StatusCode;
  bool FromSideCar;
};
struct RequestService {
  std::vector<std::string> Apis;
  std::string Name;
};
struct RequestServiceGroup {
  /* data */
  std::string Name;
  std::vector<RequestService> Services;
  RequestTags Tags;
  std::string Version;
};
struct RequestData {
  /* data */
  bool Incompatible;
  std::vector<std::string> Protocols;
  std::vector<RequestServiceGroup> ServiceGroups;
  RequestHeader Header;
};
struct RequestTags {
  /* data */
  std::vector<std::string> Value;
  bool Static;
};

struct ServiceRequestItem {
  /* data */
  std::string ServiceName;
  std::vector<std::string> Actions;
};
struct SgGroupRequest {
  /* data */
  std::string Cip;
  std::string SgName;
  std::string Version;
  RequestTags Tags;
  std::vector<std::string> Protocols;
  bool Incompatible;
  std::vector<ServiceRequestItem> Services;
};
struct V1Discovery4DockerRequest {
  std::vector<SgGroupRequest> ServiceGroups;
};

struct SeamLessItem {
  /* data */
  std::string Cip;
  std::string SgName;
  std::vector<ServiceRequestItem> Services;
};
struct SeamLessRequest {
  /* data */
  std::vector<SeamLessItem> Data;
};
} // namespace Sidecar

} // namespace Neo
