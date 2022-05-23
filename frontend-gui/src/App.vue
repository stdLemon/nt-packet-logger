<template>
  <v-app>
    <v-main>
      <NtPacketLogger />
    </v-main>
  </v-app>
</template>

<script>
import NtPacketLogger from "./components/NtPacketLogger";
export default {
  name: "App",
  components: {
    NtPacketLogger,
  },
};
</script>

<style>
html{
	overflow: hidden;
}

</style>