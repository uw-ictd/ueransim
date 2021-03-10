//
// This file is a part of UERANSIM open source project.
// Copyright (c) 2021 ALİ GÜNGÖR.
//
// The software and all associated files are licensed under GPL-3.0
// and subject to the terms and conditions defined in LICENSE file.
//

#pragma once

#include "json.hpp"
#include "octet.hpp"
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <stdexcept>

enum class EPagingDrx
{
    V32 = 32,
    V64 = 64,
    V128 = 128,
    V256 = 256
};

struct Plmn
{
    int mcc{};
    int mnc{};
    bool isLongMnc{};
};

struct SingleSlice
{
    octet sst{};
    std::optional<octet3> sd{};
};

struct NetworkSlice
{
    std::vector<SingleSlice> slices{};

    void addIfNotExists(const SingleSlice &slice);
};

enum class PduSessionType
{
    IPv4,
    IPv6,
    IPv4v6,
    ETHERNET,
    UNSTRUCTURED
};

struct PlmnSupport
{
    Plmn plmn{};
    NetworkSlice sliceSupportList{};
};

struct GutiMobileIdentity
{
    Plmn plmn;         // Not used in TMSI
    octet amfRegionId; // Not used in TMSI
    int amfSetId;      // 10-bit
    int amfPointer;    // 6-bit
    octet4 tmsi;

    GutiMobileIdentity() : plmn{}, amfRegionId{}, amfSetId{}, amfPointer{}, tmsi{}
    {
    }

    GutiMobileIdentity(const Plmn &plmn, const octet &amfRegionId, int amfSetId, int amfPointer, const octet4 &tmsi)
        : plmn(plmn), amfRegionId(amfRegionId), amfSetId(amfSetId), amfPointer(amfPointer), tmsi(tmsi)
    {
    }
};

struct ImsiMobileIdentity
{
    Plmn plmn;
    std::string routingIndicator;
    int protectionSchemaId; // 4-bit
    octet homeNetworkPublicKeyIdentifier;
    std::string schemeOutput;

    ImsiMobileIdentity()
        : plmn{}, routingIndicator{}, protectionSchemaId{}, homeNetworkPublicKeyIdentifier{}, schemeOutput{}
    {
    }

    ImsiMobileIdentity(const Plmn &plmn, std::string routingIndicator, int protectionSchemaId,
                       const octet &homeNetworkPublicKeyIdentifier, std::string schemeOutput)
        : plmn(plmn), routingIndicator(std::move(routingIndicator)), protectionSchemaId(protectionSchemaId),
          homeNetworkPublicKeyIdentifier(homeNetworkPublicKeyIdentifier), schemeOutput(std::move(schemeOutput))
    {
    }
};

struct Supi
{
    std::string type;
    std::string value;

    Supi(std::string type, std::string value) : type(std::move(type)), value(std::move(value))
    {
    }

    static Supi Parse(const std::string &supi);
};

enum class EDeregCause
{
    UNSPECIFIED,
    SWITCH_OFF,
    USIM_REMOVAL,
    DISABLE_5G,
    ECALL_INACTIVITY,
};

Json ToJson(const Supi &v);
Json ToJson(const Plmn &v);
Json ToJson(const SingleSlice &v);
Json ToJson(const NetworkSlice &v);
Json ToJson(const PlmnSupport &v);
Json ToJson(const EDeregCause &v);

bool operator==(const SingleSlice &lhs, const SingleSlice &rhs);
