#pragma once

#include "tools/process.hpp"
#include "tools/memory.hpp"
#include "outputDevice/outputDevice.hpp"

#include <detours/detours.h>
#include <array>
#include <memory>
#include <iostream>

namespace PacketLogger
{
	constexpr auto SEND_PACKET_PATTERN = std::to_array<uint8_t>({0x53, 0x56, 0x8B, 0xF2, 0x8B, 0xD8, 0xEB, 0x04});
	constexpr auto RECEIVE_PACKET_PATTERN = std::to_array<uint8_t>({0x55, 0x8B, 0xEC, 0x83, 0xC4, 0xF4, 0x53, 0x56, 0x57, 0x33, 0xC9, 0x89, 0x4D, 0xF4, 0x89, 0x55, 0xFC, 0x8B, 0xD8, 0x8B, 0x45, 0xFC});

	void HookedSendPacketWrapper();
	void HookedReceivePacketWrapper();
	
	class PacketLoggerContext
	{
	public:
		friend void HookedSendPacketWrapper();
		friend void HookedReceivePacketWrapper();

		PacketLoggerContext(PacketLoggerContext const&) = delete;
		void operator=(PacketLoggerContext const&) = delete;

		static PacketLoggerContext& GetInstance()
		{
			static PacketLoggerContext instance;
			return instance;
		}

		void Start(const std::shared_ptr<OutputDevice> &outputDevice)
		{
			this->outputDevice = outputDevice;
			auto mainModuleInfo = Tools::Process::GetModuleInfo();

			fncSendPacket = Tools::Memory::FindBytePattern(SEND_PACKET_PATTERN, (size_t)mainModuleInfo.lpBaseOfDll, mainModuleInfo.SizeOfImage);
			fncReceivePacket = Tools::Memory::FindBytePattern(RECEIVE_PACKET_PATTERN, (size_t)mainModuleInfo.lpBaseOfDll, mainModuleInfo.SizeOfImage);

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach((void **)&fncSendPacket, HookedSendPacketWrapper);
			DetourAttach((void **)&fncReceivePacket, HookedReceivePacketWrapper);
			DetourTransactionCommit();
		}

		void Stop()
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach((void **)&fncSendPacket, HookedSendPacketWrapper);
			DetourDetach((void **)&fncReceivePacket, HookedReceivePacketWrapper);
			DetourTransactionCommit();
			outputDevice.reset();
		}

	private:
		PacketLoggerContext() {}

		void HookedSendPacket(const char *packet)
		{
			std::string msg(packet);
			outputDevice->Write("SEND " + msg + '\n');
		}

		void HookedReceivePacket(const char *packet)
		{
			std::string msg(packet);
			outputDevice->Write("RECV " + msg);
		}

	private:
		std::shared_ptr<OutputDevice> outputDevice;
		void *fncSendPacket = nullptr;
		void *fncReceivePacket = nullptr;
	};

	
	/*
		Delphi (Borland fastcall)
		eax = this
		[edx] = packet
		[edx-4] = length
		[edx-8] = ref count
	*/
	__declspec(naked) void HookedSendPacketWrapper()
	{
		_asm
		{
			push ebp
			mov	ebp, esp
			sub esp, __LOCAL_SIZE
		}

		void* _this;
		char *packet;

		PacketLoggerContext *context;
		void *fncSendPacket;

		_asm
		{
			mov [_this], eax
			mov [packet], edx
		}
		
		context = &PacketLoggerContext::GetInstance();
		fncSendPacket = context->fncSendPacket;
		context->HookedSendPacket(packet);
		
		_asm
		{	
			mov eax, [_this]
			mov edx, [packet]
			call [fncSendPacket]
			mov	esp, ebp
			pop	ebp
			ret
		}
	}

	__declspec(naked) void HookedReceivePacketWrapper()
	{
		_asm
		{
			push ebp
			mov	ebp, esp
			sub esp, __LOCAL_SIZE
		}

		void* _this;
		char *packet;

		PacketLoggerContext *context;
		void *fncSendPacket;

		_asm
		{
			mov[_this], eax
			mov[packet], edx
		}

		context = &PacketLoggerContext::GetInstance();
		fncSendPacket = context->fncReceivePacket;
		context->HookedReceivePacket(packet);

		_asm
		{
			mov eax, [_this]
			mov edx, [packet]
			call[fncSendPacket]
			mov	esp, ebp
			pop	ebp
			ret
		}
	}
}