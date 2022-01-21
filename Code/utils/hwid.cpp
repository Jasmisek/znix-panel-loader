#include "hwid.h"
#pragma warning (disable: 4474)
#pragma warning (disable: 4172)
static std::string to_bitchonly(const std::wstring& str, const std::locale& loc = std::locale{})
{
    std::vector<char> buf(str.size());
    std::use_facet<std::ctype<wchar_t>>(loc).narrow(str.data(), str.data() + str.size(), '?', buf.data());

    return std::string(buf.data(), buf.size());
}
std::string hwid::get_hardware_id(const std::string id)
{
    HANDLE h_token = nullptr;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h_token))
    {
        GetLastError();
        return nullptr;
    }
    DWORD dw_buffer_size = 0;
    if (!GetTokenInformation(h_token, TokenUser, nullptr, 0, &dw_buffer_size) && (GetLastError() !=
        ERROR_INSUFFICIENT_BUFFER))
    {
        GetLastError();
        CloseHandle(h_token);
        h_token = nullptr;
        return nullptr;
    }
    std::vector<BYTE> buffer;
    buffer.resize(dw_buffer_size);
    const auto p_token_user = reinterpret_cast<PTOKEN_USER>(&buffer[0]);
    if (!GetTokenInformation(h_token, TokenUser, p_token_user, dw_buffer_size, &dw_buffer_size))
    {
        GetLastError();

        CloseHandle(h_token);
        h_token = nullptr;

        return nullptr;
    }
    if (!IsValidSid(p_token_user->User.Sid))
    {
        CloseHandle(h_token);
        h_token = nullptr;

        return nullptr;
    }
    CloseHandle(h_token);
    h_token = nullptr;
    LPSTR psz_sid = nullptr;
    if (!ConvertSidToStringSidA(p_token_user->User.Sid, &psz_sid))
    {
        return nullptr;
    }
    std::string psz_sid_str(psz_sid);
    psz_sid_str += id;
    return std::string(psz_sid_str);
}