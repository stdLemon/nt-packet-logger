<template>
  <v-simple-table height="50vh"
    >>
    <template v-slot:default>
      <thead>
        <tr>
          <th>Type</th>
          <th>Arguments</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="(packet, index) in packets" :key="index">
          <td>{{ packet.type }}</td>
          <td>{{ packet.args }}</td>
        </tr>
      </tbody>
    </template>
  </v-simple-table>
</template>

<script>
import net from "net";
export default {
  name: "HelloWorld",
  data() {
    return {
      packets: [],
    };
  },

  methods: {},
  mounted() {
    console.log("mounted");
    const server = net.createServer();

    server.listen(420, () => {
      console.log("server bound");
    });

    server.on("connection", (sock) => {
      console.log("CONNECTED: " + sock.remoteAddress + ":" + sock.remotePort);

      sock.on("data", (data) => {
        console.log(data);

        const [type, args] = data.toString().split(" ");

        this.packets.push({ type, args });
      });
    });
  },
};
</script>

<style>
::-webkit-scrollbar {
  width: 15px;
}

::-webkit-scrollbar-track {
  background: #202020;
  border-left: 1px solid #2c2c2c;
}

::-webkit-scrollbar-thumb {
  background: #3e3e3e;
  border: solid 3px #202020;
  border-radius: 7px;
}

::-webkit-scrollbar-thumb:hover {
  background: white;
}
</style>