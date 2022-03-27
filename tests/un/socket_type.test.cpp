#include <catch2/catch.hpp>
#include <lpipp/un.hpp>

using namespace lpipp;
using namespace un_constants;

TEST_CASE("unix socket type bitor operations") {
  CHECK(static_cast<std::uint32_t>(Stream) == static_cast<std::uint32_t>(SOCK_STREAM));
  CHECK(static_cast<std::uint32_t>(Stream | NonBlock) == static_cast<std::uint32_t>(SOCK_STREAM | SOCK_NONBLOCK));
  CHECK(static_cast<std::uint32_t>(NonBlock | Stream) == static_cast<std::uint32_t>(SOCK_STREAM | SOCK_NONBLOCK));
  CHECK(static_cast<std::uint32_t>(Stream | NonBlock | CloseOnExec) == static_cast<std::uint32_t>(SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(Stream | CloseOnExec | NonBlock) == static_cast<std::uint32_t>(SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(NonBlock | Stream | CloseOnExec) == static_cast<std::uint32_t>(SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(NonBlock | CloseOnExec | Stream) == static_cast<std::uint32_t>(SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(CloseOnExec | Stream | NonBlock) == static_cast<std::uint32_t>(SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(CloseOnExec | NonBlock | Stream) == static_cast<std::uint32_t>(SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(Dgram) == static_cast<std::uint32_t>(SOCK_DGRAM));
  CHECK(static_cast<std::uint32_t>(Dgram | NonBlock) == static_cast<std::uint32_t>(SOCK_DGRAM | SOCK_NONBLOCK));
  CHECK(static_cast<std::uint32_t>(NonBlock | Dgram) == static_cast<std::uint32_t>(SOCK_DGRAM | SOCK_NONBLOCK));
  CHECK(static_cast<std::uint32_t>(Dgram | NonBlock | CloseOnExec) == static_cast<std::uint32_t>(SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(Dgram | CloseOnExec | NonBlock) == static_cast<std::uint32_t>(SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(NonBlock | Dgram | CloseOnExec) == static_cast<std::uint32_t>(SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(NonBlock | CloseOnExec | Dgram) == static_cast<std::uint32_t>(SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(CloseOnExec | Dgram | NonBlock) == static_cast<std::uint32_t>(SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(CloseOnExec | NonBlock | Dgram) == static_cast<std::uint32_t>(SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(SeqPacket) == static_cast<std::uint32_t>(SOCK_SEQPACKET));
  CHECK(static_cast<std::uint32_t>(SeqPacket | NonBlock) == static_cast<std::uint32_t>(SOCK_SEQPACKET | SOCK_NONBLOCK));
  CHECK(static_cast<std::uint32_t>(NonBlock | SeqPacket) == static_cast<std::uint32_t>(SOCK_SEQPACKET | SOCK_NONBLOCK));
  CHECK(static_cast<std::uint32_t>(SeqPacket | NonBlock | CloseOnExec) == static_cast<std::uint32_t>(SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(SeqPacket | CloseOnExec | NonBlock) == static_cast<std::uint32_t>(SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(NonBlock | SeqPacket | CloseOnExec) == static_cast<std::uint32_t>(SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(NonBlock | CloseOnExec | SeqPacket) == static_cast<std::uint32_t>(SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(CloseOnExec | SeqPacket | NonBlock) == static_cast<std::uint32_t>(SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC));
  CHECK(static_cast<std::uint32_t>(CloseOnExec | NonBlock | SeqPacket) == static_cast<std::uint32_t>(SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC));
}
