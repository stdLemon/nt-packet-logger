<template>
  <v-simple-table height="100vh">
    <template v-slot:default>
      <tbody>
        <tr v-for="(packet, index) in packets" :key="index">
          <td>{{ packet.direction }}</td>
          <td>{{ packet.type }}</td>
          <td>{{ packet.args }}</td>
        </tr>
      </tbody>
    </template>
  </v-simple-table>
</template>
<script>
export default {
  name: "PacketList",
  props: ["packets"],
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
  background: #9e9898;
  border: solid 3px #272626;
  border-radius: 7px;
}

::-webkit-scrollbar-thumb:hover {
  background: white;
}
</style>