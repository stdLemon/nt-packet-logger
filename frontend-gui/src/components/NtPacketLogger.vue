<template>
  <v-container>
    <v-row>
      <v-col>
        <PacketList :packets="packets" />
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <v-btn @click="ClearPackets" block x-large color="#8f2d24">
          clear
        </v-btn>
      </v-col>
    </v-row>
  </v-container>
</template>
<script>
import PacketList from "./PacketList";
import net from "net";

export default {
  name: "NtPacketLogger",

  components: {
    PacketList,
  },

  methods: {
    ClearPackets() {
      this.packets = [];
    },
  },

  data() {
    return {
      packets: [],
      cp1250Decoder: new TextDecoder("cp1250")
    };
  },

  destroyed() {
    this.server.close();
  },

  mounted() {
    const pipeName = "\\\\.\\pipe\\nt_packet_logger";
    window.packets = this.packets;

    this.server = net.createServer();
    this.server.maxConnections = 1;

    this.server.listen(pipeName);

    this.server.on("connection", (socket) => {
      socket.on("data", (data) => {
        const decodedPackets = this.cp1250Decoder.decode(data);
        const newPackets = decodedPackets.split("\n");
        newPackets.pop();
        newPackets.forEach((packetStr) => {
          const tokens = packetStr.split(" ");
          const [direction, type] = tokens.slice(0, 2);
          const args = tokens.slice(2).join(" ");

          const packet = { direction, type, args };

          this.packets.push(packet);
        });
      });
    });
  },
};
</script>