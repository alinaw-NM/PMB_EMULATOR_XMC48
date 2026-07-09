<?xml version="1.0" encoding="ASCII"?>
<ResourceModel:App xmi:version="2.0" xmlns:xmi="http://www.omg.org/XMI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:ResourceModel="http://www.infineon.com/Davex/Resource.ecore" name="MULTICAN_CONFIG" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0" description="Configures MultiCAN peripheral" mode="NOTSHARABLE" version="4.0.14" minDaveVersion="4.1.2" instanceLabel="MULTICAN_CONFIG_0" appLabel="">
  <properties singleton="true" provideInit="true"/>
  <virtualSignals name="rx_pin_node0" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_rxd0" hwSignal="rxd" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="tx_pin_node0" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_txd0" hwSignal="txd" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="rx_pin_node1" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_rxd1" hwSignal="rxd" hwResource="//@hwResources.1" required="false" visible="true"/>
  <virtualSignals name="tx_pin_node1" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_txd1" hwSignal="txd" hwResource="//@hwResources.1" required="false" visible="true"/>
  <virtualSignals name="rx_pin_node2" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_rxd2" hwSignal="rxd" hwResource="//@hwResources.2" required="false" visible="true"/>
  <virtualSignals name="tx_pin_node2" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_txd2" hwSignal="txd" hwResource="//@hwResources.2" required="false" visible="true"/>
  <virtualSignals name="rx_pin_node3" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_rxd3" hwSignal="rxd" hwResource="//@hwResources.3" required="false" visible="true"/>
  <virtualSignals name="tx_pin_node3" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_txd3" hwSignal="txd" hwResource="//@hwResources.3" required="false" visible="true"/>
  <virtualSignals name="rx_pin_node4" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_rxd4" hwSignal="rxd" hwResource="//@hwResources.4" required="false" visible="true"/>
  <virtualSignals name="tx_pin_node4" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_txd4" hwSignal="txd" hwResource="//@hwResources.4" required="false" visible="true"/>
  <virtualSignals name="rx_pin_node5" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_rxd5" hwSignal="rxd" hwResource="//@hwResources.5" required="false" visible="true"/>
  <virtualSignals name="tx_pin_node5" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_txd5" hwSignal="txd" hwResource="//@hwResources.5" required="false" visible="true"/>
  <virtualSignals name="event_node_alert0" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_alinp0" hwSignal="alinp" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="event_node_lec_error0" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_lecinp0" hwSignal="lecinp" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="event_node_transfer_ok0" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_trinp0" hwSignal="trinp" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="event_node_frame_count0" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_cfcinp0" hwSignal="cfcinp" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="event_node_alert1" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_alinp1" hwSignal="alinp" hwResource="//@hwResources.1" required="false" visible="true"/>
  <virtualSignals name="event_node_lec_error1" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_lecinp1" hwSignal="lecinp" hwResource="//@hwResources.1" required="false" visible="true"/>
  <virtualSignals name="event_node_transfer_ok1" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_trinp1" hwSignal="trinp" hwResource="//@hwResources.1" required="false" visible="true"/>
  <virtualSignals name="event_node_frame_count1" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_cfcinp1" hwSignal="cfcinp" hwResource="//@hwResources.1" required="false" visible="true"/>
  <virtualSignals name="event_node_alert2" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_alinp2" hwSignal="alinp" hwResource="//@hwResources.2" required="false" visible="true"/>
  <virtualSignals name="event_node_lec_error2" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_lecinp2" hwSignal="lecinp" hwResource="//@hwResources.2" required="false" visible="true"/>
  <virtualSignals name="event_node_transfer_ok2" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_trinp2" hwSignal="trinp" hwResource="//@hwResources.2" required="false" visible="true"/>
  <virtualSignals name="event_node_frame_count2" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_cfcinp2" hwSignal="cfcinp" hwResource="//@hwResources.2" required="false" visible="true"/>
  <virtualSignals name="event_node_alert3" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_alinp3" hwSignal="alinp" hwResource="//@hwResources.3" required="false" visible="true"/>
  <virtualSignals name="event_node_lec_error3" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_lecinp3" hwSignal="lecinp" hwResource="//@hwResources.3" required="false" visible="true"/>
  <virtualSignals name="event_node_transfer_ok3" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_trinp3" hwSignal="trinp" hwResource="//@hwResources.3" required="false" visible="true"/>
  <virtualSignals name="event_node_frame_count3" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_cfcinp3" hwSignal="cfcinp" hwResource="//@hwResources.3" required="false" visible="true"/>
  <virtualSignals name="event_node_alert4" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_alinp4" hwSignal="alinp" hwResource="//@hwResources.4" required="false" visible="true"/>
  <virtualSignals name="event_node_lec_error4" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_lecinp4" hwSignal="lecinp" hwResource="//@hwResources.4" required="false" visible="true"/>
  <virtualSignals name="event_node_transfer_ok4" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_trinp4" hwSignal="trinp" hwResource="//@hwResources.4" required="false" visible="true"/>
  <virtualSignals name="event_node_frame_count4" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_cfcinp4" hwSignal="cfcinp" hwResource="//@hwResources.4" required="false" visible="true"/>
  <virtualSignals name="event_node_alert5" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_alinp5" hwSignal="alinp" hwResource="//@hwResources.5" required="false" visible="true"/>
  <virtualSignals name="event_node_lec_error5" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_lecinp5" hwSignal="lecinp" hwResource="//@hwResources.5" required="false" visible="true"/>
  <virtualSignals name="event_node_transfer_ok5" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_trinp5" hwSignal="trinp" hwResource="//@hwResources.5" required="false" visible="true"/>
  <virtualSignals name="event_node_frame_count5" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_can_cfcinp5" hwSignal="cfcinp" hwResource="//@hwResources.5" required="false" visible="true"/>
  <virtualSignals name="event_lmo_00_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message0_txinp" hwSignal="txinp" hwResource="//@hwResources.6" visible="true"/>
  <virtualSignals name="event_lmo_00_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message0_rxinp" hwSignal="rxinp" hwResource="//@hwResources.6" visible="true"/>
  <virtualSignals name="event_lmo_01_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message1_txinp" hwSignal="txinp" hwResource="//@hwResources.7" visible="true"/>
  <virtualSignals name="event_lmo_01_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message1_rxinp" hwSignal="rxinp" hwResource="//@hwResources.7" visible="true"/>
  <virtualSignals name="event_lmo_02_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message2_txinp" hwSignal="txinp" hwResource="//@hwResources.8" required="false" visible="true"/>
  <virtualSignals name="event_lmo_02_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message2_rxinp" hwSignal="rxinp" hwResource="//@hwResources.8" required="false" visible="true"/>
  <virtualSignals name="event_lmo_03_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message3_txinp" hwSignal="txinp" hwResource="//@hwResources.9" required="false" visible="true"/>
  <virtualSignals name="event_lmo_03_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message3_rxinp" hwSignal="rxinp" hwResource="//@hwResources.9" required="false" visible="true"/>
  <virtualSignals name="event_lmo_04_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message4_txinp" hwSignal="txinp" hwResource="//@hwResources.10" required="false" visible="true"/>
  <virtualSignals name="event_lmo_04_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message4_rxinp" hwSignal="rxinp" hwResource="//@hwResources.10" required="false" visible="true"/>
  <virtualSignals name="event_lmo_05_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message5_txinp" hwSignal="txinp" hwResource="//@hwResources.11" required="false" visible="true"/>
  <virtualSignals name="event_lmo_05_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message5_rxinp" hwSignal="rxinp" hwResource="//@hwResources.11" required="false" visible="true"/>
  <virtualSignals name="event_lmo_06_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message6_txinp" hwSignal="txinp" hwResource="//@hwResources.12" required="false" visible="true"/>
  <virtualSignals name="event_lmo_06_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message6_rxinp" hwSignal="rxinp" hwResource="//@hwResources.12" required="false" visible="true"/>
  <virtualSignals name="event_lmo_07_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message7_txinp" hwSignal="txinp" hwResource="//@hwResources.13" required="false" visible="true"/>
  <virtualSignals name="event_lmo_07_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message7_rxinp" hwSignal="rxinp" hwResource="//@hwResources.13" required="false" visible="true"/>
  <virtualSignals name="event_lmo_08_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message8_txinp" hwSignal="txinp" hwResource="//@hwResources.14" required="false" visible="true"/>
  <virtualSignals name="event_lmo_08_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message8_rxinp" hwSignal="rxinp" hwResource="//@hwResources.14" required="false" visible="true"/>
  <virtualSignals name="event_lmo_09_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message9_txinp" hwSignal="txinp" hwResource="//@hwResources.15" required="false" visible="true"/>
  <virtualSignals name="event_lmo_09_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message9_rxinp" hwSignal="rxinp" hwResource="//@hwResources.15" required="false" visible="true"/>
  <virtualSignals name="event_lmo_10_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message10_txinp" hwSignal="txinp" hwResource="//@hwResources.16" required="false" visible="true"/>
  <virtualSignals name="event_lmo_10_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message10_rxinp" hwSignal="rxinp" hwResource="//@hwResources.16" required="false" visible="true"/>
  <virtualSignals name="event_lmo_11_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message11_txinp" hwSignal="txinp" hwResource="//@hwResources.17" required="false" visible="true"/>
  <virtualSignals name="event_lmo_11_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message11_rxinp" hwSignal="rxinp" hwResource="//@hwResources.17" required="false" visible="true"/>
  <virtualSignals name="event_lmo_12_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message12_txinp" hwSignal="txinp" hwResource="//@hwResources.18" required="false" visible="true"/>
  <virtualSignals name="event_lmo_12_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message12_rxinp" hwSignal="rxinp" hwResource="//@hwResources.18" required="false" visible="true"/>
  <virtualSignals name="event_lmo_13_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message13_txinp" hwSignal="txinp" hwResource="//@hwResources.19" required="false" visible="true"/>
  <virtualSignals name="event_lmo_13_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message13_rxinp" hwSignal="rxinp" hwResource="//@hwResources.19" required="false" visible="true"/>
  <virtualSignals name="event_lmo_14_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message14_txinp" hwSignal="txinp" hwResource="//@hwResources.20" required="false" visible="true"/>
  <virtualSignals name="event_lmo_14_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message14_rxinp" hwSignal="rxinp" hwResource="//@hwResources.20" required="false" visible="true"/>
  <virtualSignals name="event_lmo_15_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message15_txinp" hwSignal="txinp" hwResource="//@hwResources.21" required="false" visible="true"/>
  <virtualSignals name="event_lmo_15_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message15_rxinp" hwSignal="rxinp" hwResource="//@hwResources.21" required="false" visible="true"/>
  <virtualSignals name="event_lmo_16_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message16_txinp" hwSignal="txinp" hwResource="//@hwResources.22" required="false" visible="true"/>
  <virtualSignals name="event_lmo_16_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message16_rxinp" hwSignal="rxinp" hwResource="//@hwResources.22" required="false" visible="true"/>
  <virtualSignals name="event_lmo_17_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message17_txinp" hwSignal="txinp" hwResource="//@hwResources.23" required="false" visible="true"/>
  <virtualSignals name="event_lmo_17_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message17_rxinp" hwSignal="rxinp" hwResource="//@hwResources.23" required="false" visible="true"/>
  <virtualSignals name="event_lmo_18_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message18_txinp" hwSignal="txinp" hwResource="//@hwResources.24" required="false" visible="true"/>
  <virtualSignals name="event_lmo_18_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message18_rxinp" hwSignal="rxinp" hwResource="//@hwResources.24" required="false" visible="true"/>
  <virtualSignals name="event_lmo_19_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message19_txinp" hwSignal="txinp" hwResource="//@hwResources.25" required="false" visible="true"/>
  <virtualSignals name="event_lmo_19_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message19_rxinp" hwSignal="rxinp" hwResource="//@hwResources.25" required="false" visible="true"/>
  <virtualSignals name="event_lmo_20_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message20_txinp" hwSignal="txinp" hwResource="//@hwResources.26" required="false" visible="true"/>
  <virtualSignals name="event_lmo_20_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message20_rxinp" hwSignal="rxinp" hwResource="//@hwResources.26" required="false" visible="true"/>
  <virtualSignals name="event_lmo_21_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message21_txinp" hwSignal="txinp" hwResource="//@hwResources.27" required="false" visible="true"/>
  <virtualSignals name="event_lmo_21_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message21_rxinp" hwSignal="rxinp" hwResource="//@hwResources.27" required="false" visible="true"/>
  <virtualSignals name="event_lmo_22_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message22_txinp" hwSignal="txinp" hwResource="//@hwResources.28" required="false" visible="true"/>
  <virtualSignals name="event_lmo_22_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message22_rxinp" hwSignal="rxinp" hwResource="//@hwResources.28" required="false" visible="true"/>
  <virtualSignals name="event_lmo_23_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message23_txinp" hwSignal="txinp" hwResource="//@hwResources.29" required="false" visible="true"/>
  <virtualSignals name="event_lmo_23_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message23_rxinp" hwSignal="rxinp" hwResource="//@hwResources.29" required="false" visible="true"/>
  <virtualSignals name="event_lmo_24_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message24_txinp" hwSignal="txinp" hwResource="//@hwResources.30" required="false" visible="true"/>
  <virtualSignals name="event_lmo_24_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message24_rxinp" hwSignal="rxinp" hwResource="//@hwResources.30" required="false" visible="true"/>
  <virtualSignals name="event_lmo_25_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message25_txinp" hwSignal="txinp" hwResource="//@hwResources.31" required="false" visible="true"/>
  <virtualSignals name="event_lmo_25_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message25_rxinp" hwSignal="rxinp" hwResource="//@hwResources.31" required="false" visible="true"/>
  <virtualSignals name="event_lmo_26_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message26_txinp" hwSignal="txinp" hwResource="//@hwResources.32" required="false" visible="true"/>
  <virtualSignals name="event_lmo_26_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message26_rxinp" hwSignal="rxinp" hwResource="//@hwResources.32" required="false" visible="true"/>
  <virtualSignals name="event_lmo_27_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message27_txinp" hwSignal="txinp" hwResource="//@hwResources.33" required="false" visible="true"/>
  <virtualSignals name="event_lmo_27_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message27_rxinp" hwSignal="rxinp" hwResource="//@hwResources.33" required="false" visible="true"/>
  <virtualSignals name="event_lmo_28_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message28_txinp" hwSignal="txinp" hwResource="//@hwResources.34" required="false" visible="true"/>
  <virtualSignals name="event_lmo_28_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message28_rxinp" hwSignal="rxinp" hwResource="//@hwResources.34" required="false" visible="true"/>
  <virtualSignals name="event_lmo_29_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message29_txinp" hwSignal="txinp" hwResource="//@hwResources.35" required="false" visible="true"/>
  <virtualSignals name="event_lmo_29_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message29_rxinp" hwSignal="rxinp" hwResource="//@hwResources.35" required="false" visible="true"/>
  <virtualSignals name="event_lmo_30_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message30_txinp" hwSignal="txinp" hwResource="//@hwResources.36" required="false" visible="true"/>
  <virtualSignals name="event_lmo_30_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message30_rxinp" hwSignal="rxinp" hwResource="//@hwResources.36" required="false" visible="true"/>
  <virtualSignals name="event_lmo_31_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message31_txinp" hwSignal="txinp" hwResource="//@hwResources.37" required="false" visible="true"/>
  <virtualSignals name="event_lmo_31_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message31_rxinp" hwSignal="rxinp" hwResource="//@hwResources.37" required="false" visible="true"/>
  <virtualSignals name="event_lmo_32_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message32_txinp" hwSignal="txinp" hwResource="//@hwResources.38" required="false" visible="true"/>
  <virtualSignals name="event_lmo_32_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message32_rxinp" hwSignal="rxinp" hwResource="//@hwResources.38" required="false" visible="true"/>
  <virtualSignals name="event_lmo_33_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message33_txinp" hwSignal="txinp" hwResource="//@hwResources.39" required="false" visible="true"/>
  <virtualSignals name="event_lmo_33_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message33_rxinp" hwSignal="rxinp" hwResource="//@hwResources.39" required="false" visible="true"/>
  <virtualSignals name="event_lmo_34_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message34_txinp" hwSignal="txinp" hwResource="//@hwResources.40" required="false" visible="true"/>
  <virtualSignals name="event_lmo_34_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message34_rxinp" hwSignal="rxinp" hwResource="//@hwResources.40" required="false" visible="true"/>
  <virtualSignals name="event_lmo_35_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message35_txinp" hwSignal="txinp" hwResource="//@hwResources.41" required="false" visible="true"/>
  <virtualSignals name="event_lmo_35_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message35_rxinp" hwSignal="rxinp" hwResource="//@hwResources.41" required="false" visible="true"/>
  <virtualSignals name="event_lmo_36_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message36_txinp" hwSignal="txinp" hwResource="//@hwResources.42" required="false" visible="true"/>
  <virtualSignals name="event_lmo_36_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message36_rxinp" hwSignal="rxinp" hwResource="//@hwResources.42" required="false" visible="true"/>
  <virtualSignals name="event_lmo_37_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message37_txinp" hwSignal="txinp" hwResource="//@hwResources.43" required="false" visible="true"/>
  <virtualSignals name="event_lmo_37_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message37_rxinp" hwSignal="rxinp" hwResource="//@hwResources.43" required="false" visible="true"/>
  <virtualSignals name="event_lmo_38_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message38_txinp" hwSignal="txinp" hwResource="//@hwResources.44" required="false" visible="true"/>
  <virtualSignals name="event_lmo_38_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message38_rxinp" hwSignal="rxinp" hwResource="//@hwResources.44" required="false" visible="true"/>
  <virtualSignals name="event_lmo_39_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message39_txinp" hwSignal="txinp" hwResource="//@hwResources.45" required="false" visible="true"/>
  <virtualSignals name="event_lmo_39_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message39_rxinp" hwSignal="rxinp" hwResource="//@hwResources.45" required="false" visible="true"/>
  <virtualSignals name="event_lmo_40_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message40_txinp" hwSignal="txinp" hwResource="//@hwResources.46" required="false" visible="true"/>
  <virtualSignals name="event_lmo_40_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message40_rxinp" hwSignal="rxinp" hwResource="//@hwResources.46" required="false" visible="true"/>
  <virtualSignals name="event_lmo_41_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message41_txinp" hwSignal="txinp" hwResource="//@hwResources.47" required="false" visible="true"/>
  <virtualSignals name="event_lmo_41_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message41_rxinp" hwSignal="rxinp" hwResource="//@hwResources.47" required="false" visible="true"/>
  <virtualSignals name="event_lmo_42_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message42_txinp" hwSignal="txinp" hwResource="//@hwResources.48" required="false" visible="true"/>
  <virtualSignals name="event_lmo_42_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message42_rxinp" hwSignal="rxinp" hwResource="//@hwResources.48" required="false" visible="true"/>
  <virtualSignals name="event_lmo_43_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message43_txinp" hwSignal="txinp" hwResource="//@hwResources.49" required="false" visible="true"/>
  <virtualSignals name="event_lmo_43_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message43_rxinp" hwSignal="rxinp" hwResource="//@hwResources.49" required="false" visible="true"/>
  <virtualSignals name="event_lmo_44_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message44_txinp" hwSignal="txinp" hwResource="//@hwResources.50" required="false" visible="true"/>
  <virtualSignals name="event_lmo_44_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message44_rxinp" hwSignal="rxinp" hwResource="//@hwResources.50" required="false" visible="true"/>
  <virtualSignals name="event_lmo_45_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message45_txinp" hwSignal="txinp" hwResource="//@hwResources.51" required="false" visible="true"/>
  <virtualSignals name="event_lmo_45_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message45_rxinp" hwSignal="rxinp" hwResource="//@hwResources.51" required="false" visible="true"/>
  <virtualSignals name="event_lmo_46_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message46_txinp" hwSignal="txinp" hwResource="//@hwResources.52" required="false" visible="true"/>
  <virtualSignals name="event_lmo_46_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message46_rxinp" hwSignal="rxinp" hwResource="//@hwResources.52" required="false" visible="true"/>
  <virtualSignals name="event_lmo_47_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message47_txinp" hwSignal="txinp" hwResource="//@hwResources.53" required="false" visible="true"/>
  <virtualSignals name="event_lmo_47_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message47_rxinp" hwSignal="rxinp" hwResource="//@hwResources.53" required="false" visible="true"/>
  <virtualSignals name="event_lmo_48_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message48_txinp" hwSignal="txinp" hwResource="//@hwResources.54" required="false" visible="true"/>
  <virtualSignals name="event_lmo_48_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message48_rxinp" hwSignal="rxinp" hwResource="//@hwResources.54" required="false" visible="true"/>
  <virtualSignals name="event_lmo_49_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message49_txinp" hwSignal="txinp" hwResource="//@hwResources.55" required="false" visible="true"/>
  <virtualSignals name="event_lmo_49_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message49_rxinp" hwSignal="rxinp" hwResource="//@hwResources.55" required="false" visible="true"/>
  <virtualSignals name="event_lmo_50_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message50_txinp" hwSignal="txinp" hwResource="//@hwResources.56" required="false" visible="true"/>
  <virtualSignals name="event_lmo_50_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message50_rxinp" hwSignal="rxinp" hwResource="//@hwResources.56" required="false" visible="true"/>
  <virtualSignals name="event_lmo_51_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message51_txinp" hwSignal="txinp" hwResource="//@hwResources.57" required="false" visible="true"/>
  <virtualSignals name="event_lmo_51_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message51_rxinp" hwSignal="rxinp" hwResource="//@hwResources.57" required="false" visible="true"/>
  <virtualSignals name="event_lmo_52_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message52_txinp" hwSignal="txinp" hwResource="//@hwResources.58" required="false" visible="true"/>
  <virtualSignals name="event_lmo_52_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message52_rxinp" hwSignal="rxinp" hwResource="//@hwResources.58" required="false" visible="true"/>
  <virtualSignals name="event_lmo_53_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message53_txinp" hwSignal="txinp" hwResource="//@hwResources.59" required="false" visible="true"/>
  <virtualSignals name="event_lmo_53_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message53_rxinp" hwSignal="rxinp" hwResource="//@hwResources.59" required="false" visible="true"/>
  <virtualSignals name="event_lmo_54_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message54_txinp" hwSignal="txinp" hwResource="//@hwResources.60" required="false" visible="true"/>
  <virtualSignals name="event_lmo_54_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message54_rxinp" hwSignal="rxinp" hwResource="//@hwResources.60" required="false" visible="true"/>
  <virtualSignals name="event_lmo_55_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message55_txinp" hwSignal="txinp" hwResource="//@hwResources.61" required="false" visible="true"/>
  <virtualSignals name="event_lmo_55_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message55_rxinp" hwSignal="rxinp" hwResource="//@hwResources.61" required="false" visible="true"/>
  <virtualSignals name="event_lmo_56_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message56_txinp" hwSignal="txinp" hwResource="//@hwResources.62" required="false" visible="true"/>
  <virtualSignals name="event_lmo_56_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message56_rxinp" hwSignal="rxinp" hwResource="//@hwResources.62" required="false" visible="true"/>
  <virtualSignals name="event_lmo_57_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message57_txinp" hwSignal="txinp" hwResource="//@hwResources.63" required="false" visible="true"/>
  <virtualSignals name="event_lmo_57_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message57_rxinp" hwSignal="rxinp" hwResource="//@hwResources.63" required="false" visible="true"/>
  <virtualSignals name="event_lmo_58_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message58_txinp" hwSignal="txinp" hwResource="//@hwResources.64" required="false" visible="true"/>
  <virtualSignals name="event_lmo_58_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message58_rxinp" hwSignal="rxinp" hwResource="//@hwResources.64" required="false" visible="true"/>
  <virtualSignals name="event_lmo_59_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message59_txinp" hwSignal="txinp" hwResource="//@hwResources.65" required="false" visible="true"/>
  <virtualSignals name="event_lmo_59_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message59_rxinp" hwSignal="rxinp" hwResource="//@hwResources.65" required="false" visible="true"/>
  <virtualSignals name="event_lmo_60_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message60_txinp" hwSignal="txinp" hwResource="//@hwResources.66" required="false" visible="true"/>
  <virtualSignals name="event_lmo_60_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message60_rxinp" hwSignal="rxinp" hwResource="//@hwResources.66" required="false" visible="true"/>
  <virtualSignals name="event_lmo_61_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message61_txinp" hwSignal="txinp" hwResource="//@hwResources.67" required="false" visible="true"/>
  <virtualSignals name="event_lmo_61_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message61_rxinp" hwSignal="rxinp" hwResource="//@hwResources.67" required="false" visible="true"/>
  <virtualSignals name="event_lmo_62_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message62_txinp" hwSignal="txinp" hwResource="//@hwResources.68" required="false" visible="true"/>
  <virtualSignals name="event_lmo_62_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message62_rxinp" hwSignal="rxinp" hwResource="//@hwResources.68" required="false" visible="true"/>
  <virtualSignals name="event_lmo_63_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message63_txinp" hwSignal="txinp" hwResource="//@hwResources.69" required="false" visible="true"/>
  <virtualSignals name="event_lmo_63_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message63_rxinp" hwSignal="rxinp" hwResource="//@hwResources.69" required="false" visible="true"/>
  <virtualSignals name="event_lmo_64_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message64_txinp" hwSignal="txinp" hwResource="//@hwResources.70" required="false" visible="true"/>
  <virtualSignals name="event_lmo_64_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message64_rxinp" hwSignal="rxinp" hwResource="//@hwResources.70" required="false" visible="true"/>
  <virtualSignals name="event_lmo_65_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message65_txinp" hwSignal="txinp" hwResource="//@hwResources.71" required="false" visible="true"/>
  <virtualSignals name="event_lmo_65_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message65_rxinp" hwSignal="rxinp" hwResource="//@hwResources.71" required="false" visible="true"/>
  <virtualSignals name="event_lmo_66_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message66_txinp" hwSignal="txinp" hwResource="//@hwResources.72" required="false" visible="true"/>
  <virtualSignals name="event_lmo_66_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message66_rxinp" hwSignal="rxinp" hwResource="//@hwResources.72" required="false" visible="true"/>
  <virtualSignals name="event_lmo_67_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message67_txinp" hwSignal="txinp" hwResource="//@hwResources.73" required="false" visible="true"/>
  <virtualSignals name="event_lmo_67_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message67_rxinp" hwSignal="rxinp" hwResource="//@hwResources.73" required="false" visible="true"/>
  <virtualSignals name="event_lmo_68_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message68_txinp" hwSignal="txinp" hwResource="//@hwResources.74" required="false" visible="true"/>
  <virtualSignals name="event_lmo_68_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message68_rxinp" hwSignal="rxinp" hwResource="//@hwResources.74" required="false" visible="true"/>
  <virtualSignals name="event_lmo_69_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message69_txinp" hwSignal="txinp" hwResource="//@hwResources.75" required="false" visible="true"/>
  <virtualSignals name="event_lmo_69_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message69_rxinp" hwSignal="rxinp" hwResource="//@hwResources.75" required="false" visible="true"/>
  <virtualSignals name="event_lmo_70_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message70_txinp" hwSignal="txinp" hwResource="//@hwResources.76" required="false" visible="true"/>
  <virtualSignals name="event_lmo_70_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message70_rxinp" hwSignal="rxinp" hwResource="//@hwResources.76" required="false" visible="true"/>
  <virtualSignals name="event_lmo_71_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message71_txinp" hwSignal="txinp" hwResource="//@hwResources.77" required="false" visible="true"/>
  <virtualSignals name="event_lmo_71_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message71_rxinp" hwSignal="rxinp" hwResource="//@hwResources.77" required="false" visible="true"/>
  <virtualSignals name="event_lmo_72_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message72_txinp" hwSignal="txinp" hwResource="//@hwResources.78" required="false" visible="true"/>
  <virtualSignals name="event_lmo_72_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message72_rxinp" hwSignal="rxinp" hwResource="//@hwResources.78" required="false" visible="true"/>
  <virtualSignals name="event_lmo_73_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message73_txinp" hwSignal="txinp" hwResource="//@hwResources.79" required="false" visible="true"/>
  <virtualSignals name="event_lmo_73_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message73_rxinp" hwSignal="rxinp" hwResource="//@hwResources.79" required="false" visible="true"/>
  <virtualSignals name="event_lmo_74_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message74_txinp" hwSignal="txinp" hwResource="//@hwResources.80" required="false" visible="true"/>
  <virtualSignals name="event_lmo_74_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message74_rxinp" hwSignal="rxinp" hwResource="//@hwResources.80" required="false" visible="true"/>
  <virtualSignals name="event_lmo_75_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message75_txinp" hwSignal="txinp" hwResource="//@hwResources.81" required="false" visible="true"/>
  <virtualSignals name="event_lmo_75_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message75_rxinp" hwSignal="rxinp" hwResource="//@hwResources.81" required="false" visible="true"/>
  <virtualSignals name="event_lmo_76_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message76_txinp" hwSignal="txinp" hwResource="//@hwResources.82" required="false" visible="true"/>
  <virtualSignals name="event_lmo_76_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message76_rxinp" hwSignal="rxinp" hwResource="//@hwResources.82" required="false" visible="true"/>
  <virtualSignals name="event_lmo_77_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message77_txinp" hwSignal="txinp" hwResource="//@hwResources.83" required="false" visible="true"/>
  <virtualSignals name="event_lmo_77_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message77_rxinp" hwSignal="rxinp" hwResource="//@hwResources.83" required="false" visible="true"/>
  <virtualSignals name="event_lmo_78_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message78_txinp" hwSignal="txinp" hwResource="//@hwResources.84" required="false" visible="true"/>
  <virtualSignals name="event_lmo_78_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message78_rxinp" hwSignal="rxinp" hwResource="//@hwResources.84" required="false" visible="true"/>
  <virtualSignals name="event_lmo_79_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message79_txinp" hwSignal="txinp" hwResource="//@hwResources.85" required="false" visible="true"/>
  <virtualSignals name="event_lmo_79_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message79_rxinp" hwSignal="rxinp" hwResource="//@hwResources.85" required="false" visible="true"/>
  <virtualSignals name="event_lmo_80_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message80_txinp" hwSignal="txinp" hwResource="//@hwResources.86" required="false" visible="true"/>
  <virtualSignals name="event_lmo_80_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message80_rxinp" hwSignal="rxinp" hwResource="//@hwResources.86" required="false" visible="true"/>
  <virtualSignals name="event_lmo_81_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message81_txinp" hwSignal="txinp" hwResource="//@hwResources.87" required="false" visible="true"/>
  <virtualSignals name="event_lmo_81_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message81_rxinp" hwSignal="rxinp" hwResource="//@hwResources.87" required="false" visible="true"/>
  <virtualSignals name="event_lmo_82_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message82_txinp" hwSignal="txinp" hwResource="//@hwResources.88" required="false" visible="true"/>
  <virtualSignals name="event_lmo_82_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message82_rxinp" hwSignal="rxinp" hwResource="//@hwResources.88" required="false" visible="true"/>
  <virtualSignals name="event_lmo_83_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message83_txinp" hwSignal="txinp" hwResource="//@hwResources.89" required="false" visible="true"/>
  <virtualSignals name="event_lmo_83_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message83_rxinp" hwSignal="rxinp" hwResource="//@hwResources.89" required="false" visible="true"/>
  <virtualSignals name="event_lmo_84_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message84_txinp" hwSignal="txinp" hwResource="//@hwResources.90" required="false" visible="true"/>
  <virtualSignals name="event_lmo_84_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message84_rxinp" hwSignal="rxinp" hwResource="//@hwResources.90" required="false" visible="true"/>
  <virtualSignals name="event_lmo_85_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message85_txinp" hwSignal="txinp" hwResource="//@hwResources.91" required="false" visible="true"/>
  <virtualSignals name="event_lmo_85_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message85_rxinp" hwSignal="rxinp" hwResource="//@hwResources.91" required="false" visible="true"/>
  <virtualSignals name="event_lmo_86_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message86_txinp" hwSignal="txinp" hwResource="//@hwResources.92" required="false" visible="true"/>
  <virtualSignals name="event_lmo_86_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message86_rxinp" hwSignal="rxinp" hwResource="//@hwResources.92" required="false" visible="true"/>
  <virtualSignals name="event_lmo_87_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message87_txinp" hwSignal="txinp" hwResource="//@hwResources.93" required="false" visible="true"/>
  <virtualSignals name="event_lmo_87_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message87_rxinp" hwSignal="rxinp" hwResource="//@hwResources.93" required="false" visible="true"/>
  <virtualSignals name="event_lmo_88_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message88_txinp" hwSignal="txinp" hwResource="//@hwResources.94" required="false" visible="true"/>
  <virtualSignals name="event_lmo_88_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message88_rxinp" hwSignal="rxinp" hwResource="//@hwResources.94" required="false" visible="true"/>
  <virtualSignals name="event_lmo_89_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message89_txinp" hwSignal="txinp" hwResource="//@hwResources.95" required="false" visible="true"/>
  <virtualSignals name="event_lmo_89_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message89_rxinp" hwSignal="rxinp" hwResource="//@hwResources.95" required="false" visible="true"/>
  <virtualSignals name="event_lmo_90_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message90_txinp" hwSignal="txinp" hwResource="//@hwResources.96" required="false" visible="true"/>
  <virtualSignals name="event_lmo_90_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message90_rxinp" hwSignal="rxinp" hwResource="//@hwResources.96" required="false" visible="true"/>
  <virtualSignals name="event_lmo_91_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message91_txinp" hwSignal="txinp" hwResource="//@hwResources.97" required="false" visible="true"/>
  <virtualSignals name="event_lmo_91_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message91_rxinp" hwSignal="rxinp" hwResource="//@hwResources.97" required="false" visible="true"/>
  <virtualSignals name="event_lmo_92_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message92_txinp" hwSignal="txinp" hwResource="//@hwResources.98" required="false" visible="true"/>
  <virtualSignals name="event_lmo_92_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message92_rxinp" hwSignal="rxinp" hwResource="//@hwResources.98" required="false" visible="true"/>
  <virtualSignals name="event_lmo_93_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message93_txinp" hwSignal="txinp" hwResource="//@hwResources.99" required="false" visible="true"/>
  <virtualSignals name="event_lmo_93_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message93_rxinp" hwSignal="rxinp" hwResource="//@hwResources.99" required="false" visible="true"/>
  <virtualSignals name="event_lmo_94_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message94_txinp" hwSignal="txinp" hwResource="//@hwResources.100" required="false" visible="true"/>
  <virtualSignals name="event_lmo_94_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message94_rxinp" hwSignal="rxinp" hwResource="//@hwResources.100" required="false" visible="true"/>
  <virtualSignals name="event_lmo_95_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message95_txinp" hwSignal="txinp" hwResource="//@hwResources.101" required="false" visible="true"/>
  <virtualSignals name="event_lmo_95_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message95_rxinp" hwSignal="rxinp" hwResource="//@hwResources.101" required="false" visible="true"/>
  <virtualSignals name="event_lmo_96_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message96_txinp" hwSignal="txinp" hwResource="//@hwResources.102" required="false" visible="true"/>
  <virtualSignals name="event_lmo_96_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message96_rxinp" hwSignal="rxinp" hwResource="//@hwResources.102" required="false" visible="true"/>
  <virtualSignals name="event_lmo_97_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message97_txinp" hwSignal="txinp" hwResource="//@hwResources.103" required="false" visible="true"/>
  <virtualSignals name="event_lmo_97_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message97_rxinp" hwSignal="rxinp" hwResource="//@hwResources.103" required="false" visible="true"/>
  <virtualSignals name="event_lmo_98_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message98_txinp" hwSignal="txinp" hwResource="//@hwResources.104" required="false" visible="true"/>
  <virtualSignals name="event_lmo_98_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message98_rxinp" hwSignal="rxinp" hwResource="//@hwResources.104" required="false" visible="true"/>
  <virtualSignals name="event_lmo_99_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message99_txinp" hwSignal="txinp" hwResource="//@hwResources.105" required="false" visible="true"/>
  <virtualSignals name="event_lmo_99_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message99_rxinp" hwSignal="rxinp" hwResource="//@hwResources.105" required="false" visible="true"/>
  <virtualSignals name="event_lmo_100_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message100_txinp" hwSignal="txinp" hwResource="//@hwResources.106" required="false" visible="true"/>
  <virtualSignals name="event_lmo_100_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message100_rxinp" hwSignal="rxinp" hwResource="//@hwResources.106" required="false" visible="true"/>
  <virtualSignals name="event_lmo_101_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message101_txinp" hwSignal="txinp" hwResource="//@hwResources.107" required="false" visible="true"/>
  <virtualSignals name="event_lmo_101_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message101_rxinp" hwSignal="rxinp" hwResource="//@hwResources.107" required="false" visible="true"/>
  <virtualSignals name="event_lmo_102_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message102_txinp" hwSignal="txinp" hwResource="//@hwResources.108" required="false" visible="true"/>
  <virtualSignals name="event_lmo_102_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message102_rxinp" hwSignal="rxinp" hwResource="//@hwResources.108" required="false" visible="true"/>
  <virtualSignals name="event_lmo_103_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message103_txinp" hwSignal="txinp" hwResource="//@hwResources.109" required="false" visible="true"/>
  <virtualSignals name="event_lmo_103_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message103_rxinp" hwSignal="rxinp" hwResource="//@hwResources.109" required="false" visible="true"/>
  <virtualSignals name="event_lmo_104_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message104_txinp" hwSignal="txinp" hwResource="//@hwResources.110" required="false" visible="true"/>
  <virtualSignals name="event_lmo_104_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message104_rxinp" hwSignal="rxinp" hwResource="//@hwResources.110" required="false" visible="true"/>
  <virtualSignals name="event_lmo_105_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message105_txinp" hwSignal="txinp" hwResource="//@hwResources.111" required="false" visible="true"/>
  <virtualSignals name="event_lmo_105_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message105_rxinp" hwSignal="rxinp" hwResource="//@hwResources.111" required="false" visible="true"/>
  <virtualSignals name="event_lmo_106_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message106_txinp" hwSignal="txinp" hwResource="//@hwResources.112" required="false" visible="true"/>
  <virtualSignals name="event_lmo_106_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message106_rxinp" hwSignal="rxinp" hwResource="//@hwResources.112" required="false" visible="true"/>
  <virtualSignals name="event_lmo_107_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message107_txinp" hwSignal="txinp" hwResource="//@hwResources.113" required="false" visible="true"/>
  <virtualSignals name="event_lmo_107_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message107_rxinp" hwSignal="rxinp" hwResource="//@hwResources.113" required="false" visible="true"/>
  <virtualSignals name="event_lmo_108_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message108_txinp" hwSignal="txinp" hwResource="//@hwResources.114" required="false" visible="true"/>
  <virtualSignals name="event_lmo_108_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message108_rxinp" hwSignal="rxinp" hwResource="//@hwResources.114" required="false" visible="true"/>
  <virtualSignals name="event_lmo_109_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message109_txinp" hwSignal="txinp" hwResource="//@hwResources.115" required="false" visible="true"/>
  <virtualSignals name="event_lmo_109_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message109_rxinp" hwSignal="rxinp" hwResource="//@hwResources.115" required="false" visible="true"/>
  <virtualSignals name="event_lmo_110_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message110_txinp" hwSignal="txinp" hwResource="//@hwResources.116" required="false" visible="true"/>
  <virtualSignals name="event_lmo_110_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message110_rxinp" hwSignal="rxinp" hwResource="//@hwResources.116" required="false" visible="true"/>
  <virtualSignals name="event_lmo_111_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message111_txinp" hwSignal="txinp" hwResource="//@hwResources.117" required="false" visible="true"/>
  <virtualSignals name="event_lmo_111_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message111_rxinp" hwSignal="rxinp" hwResource="//@hwResources.117" required="false" visible="true"/>
  <virtualSignals name="event_lmo_112_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message112_txinp" hwSignal="txinp" hwResource="//@hwResources.118" required="false" visible="true"/>
  <virtualSignals name="event_lmo_112_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message112_rxinp" hwSignal="rxinp" hwResource="//@hwResources.118" required="false" visible="true"/>
  <virtualSignals name="event_lmo_113_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message113_txinp" hwSignal="txinp" hwResource="//@hwResources.119" required="false" visible="true"/>
  <virtualSignals name="event_lmo_113_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message113_rxinp" hwSignal="rxinp" hwResource="//@hwResources.119" required="false" visible="true"/>
  <virtualSignals name="event_lmo_114_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message114_txinp" hwSignal="txinp" hwResource="//@hwResources.120" required="false" visible="true"/>
  <virtualSignals name="event_lmo_114_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message114_rxinp" hwSignal="rxinp" hwResource="//@hwResources.120" required="false" visible="true"/>
  <virtualSignals name="event_lmo_115_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message115_txinp" hwSignal="txinp" hwResource="//@hwResources.121" required="false" visible="true"/>
  <virtualSignals name="event_lmo_115_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message115_rxinp" hwSignal="rxinp" hwResource="//@hwResources.121" required="false" visible="true"/>
  <virtualSignals name="event_lmo_116_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message116_txinp" hwSignal="txinp" hwResource="//@hwResources.122" required="false" visible="true"/>
  <virtualSignals name="event_lmo_116_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message116_rxinp" hwSignal="rxinp" hwResource="//@hwResources.122" required="false" visible="true"/>
  <virtualSignals name="event_lmo_117_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message117_txinp" hwSignal="txinp" hwResource="//@hwResources.123" required="false" visible="true"/>
  <virtualSignals name="event_lmo_117_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message117_rxinp" hwSignal="rxinp" hwResource="//@hwResources.123" required="false" visible="true"/>
  <virtualSignals name="event_lmo_118_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message118_txinp" hwSignal="txinp" hwResource="//@hwResources.124" required="false" visible="true"/>
  <virtualSignals name="event_lmo_118_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message118_rxinp" hwSignal="rxinp" hwResource="//@hwResources.124" required="false" visible="true"/>
  <virtualSignals name="event_lmo_119_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message119_txinp" hwSignal="txinp" hwResource="//@hwResources.125" required="false" visible="true"/>
  <virtualSignals name="event_lmo_119_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message119_rxinp" hwSignal="rxinp" hwResource="//@hwResources.125" required="false" visible="true"/>
  <virtualSignals name="event_lmo_120_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message120_txinp" hwSignal="txinp" hwResource="//@hwResources.126" required="false" visible="true"/>
  <virtualSignals name="event_lmo_120_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message120_rxinp" hwSignal="rxinp" hwResource="//@hwResources.126" required="false" visible="true"/>
  <virtualSignals name="event_lmo_121_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message121_txinp" hwSignal="txinp" hwResource="//@hwResources.127" required="false" visible="true"/>
  <virtualSignals name="event_lmo_121_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message121_rxinp" hwSignal="rxinp" hwResource="//@hwResources.127" required="false" visible="true"/>
  <virtualSignals name="event_lmo_122_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message122_txinp" hwSignal="txinp" hwResource="//@hwResources.128" required="false" visible="true"/>
  <virtualSignals name="event_lmo_122_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message122_rxinp" hwSignal="rxinp" hwResource="//@hwResources.128" required="false" visible="true"/>
  <virtualSignals name="event_lmo_123_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message123_txinp" hwSignal="txinp" hwResource="//@hwResources.129" required="false" visible="true"/>
  <virtualSignals name="event_lmo_123_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message123_rxinp" hwSignal="rxinp" hwResource="//@hwResources.129" required="false" visible="true"/>
  <virtualSignals name="event_lmo_124_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message124_txinp" hwSignal="txinp" hwResource="//@hwResources.130" required="false" visible="true"/>
  <virtualSignals name="event_lmo_124_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message124_rxinp" hwSignal="rxinp" hwResource="//@hwResources.130" required="false" visible="true"/>
  <virtualSignals name="event_lmo_125_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message125_txinp" hwSignal="txinp" hwResource="//@hwResources.131" required="false" visible="true"/>
  <virtualSignals name="event_lmo_125_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message125_rxinp" hwSignal="rxinp" hwResource="//@hwResources.131" required="false" visible="true"/>
  <virtualSignals name="event_lmo_126_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message126_txinp" hwSignal="txinp" hwResource="//@hwResources.132" required="false" visible="true"/>
  <virtualSignals name="event_lmo_126_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message126_rxinp" hwSignal="rxinp" hwResource="//@hwResources.132" required="false" visible="true"/>
  <virtualSignals name="event_lmo_127_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message127_txinp" hwSignal="txinp" hwResource="//@hwResources.133" required="false" visible="true"/>
  <virtualSignals name="event_lmo_127_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message127_rxinp" hwSignal="rxinp" hwResource="//@hwResources.133" required="false" visible="true"/>
  <virtualSignals name="event_lmo_128_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message128_txinp" hwSignal="txinp" hwResource="//@hwResources.134" required="false" visible="true"/>
  <virtualSignals name="event_lmo_128_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message128_rxinp" hwSignal="rxinp" hwResource="//@hwResources.134" required="false" visible="true"/>
  <virtualSignals name="event_lmo_129_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message129_txinp" hwSignal="txinp" hwResource="//@hwResources.135" required="false" visible="true"/>
  <virtualSignals name="event_lmo_129_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message129_rxinp" hwSignal="rxinp" hwResource="//@hwResources.135" required="false" visible="true"/>
  <virtualSignals name="event_lmo_130_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message130_txinp" hwSignal="txinp" hwResource="//@hwResources.136" required="false" visible="true"/>
  <virtualSignals name="event_lmo_130_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message130_rxinp" hwSignal="rxinp" hwResource="//@hwResources.136" required="false" visible="true"/>
  <virtualSignals name="event_lmo_131_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message131_txinp" hwSignal="txinp" hwResource="//@hwResources.137" required="false" visible="true"/>
  <virtualSignals name="event_lmo_131_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message131_rxinp" hwSignal="rxinp" hwResource="//@hwResources.137" required="false" visible="true"/>
  <virtualSignals name="event_lmo_132_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message132_txinp" hwSignal="txinp" hwResource="//@hwResources.138" required="false" visible="true"/>
  <virtualSignals name="event_lmo_132_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message132_rxinp" hwSignal="rxinp" hwResource="//@hwResources.138" required="false" visible="true"/>
  <virtualSignals name="event_lmo_133_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message133_txinp" hwSignal="txinp" hwResource="//@hwResources.139" required="false" visible="true"/>
  <virtualSignals name="event_lmo_133_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message133_rxinp" hwSignal="rxinp" hwResource="//@hwResources.139" required="false" visible="true"/>
  <virtualSignals name="event_lmo_134_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message134_txinp" hwSignal="txinp" hwResource="//@hwResources.140" required="false" visible="true"/>
  <virtualSignals name="event_lmo_134_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message134_rxinp" hwSignal="rxinp" hwResource="//@hwResources.140" required="false" visible="true"/>
  <virtualSignals name="event_lmo_135_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message135_txinp" hwSignal="txinp" hwResource="//@hwResources.141" required="false" visible="true"/>
  <virtualSignals name="event_lmo_135_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message135_rxinp" hwSignal="rxinp" hwResource="//@hwResources.141" required="false" visible="true"/>
  <virtualSignals name="event_lmo_136_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message136_txinp" hwSignal="txinp" hwResource="//@hwResources.142" required="false" visible="true"/>
  <virtualSignals name="event_lmo_136_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message136_rxinp" hwSignal="rxinp" hwResource="//@hwResources.142" required="false" visible="true"/>
  <virtualSignals name="event_lmo_137_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message137_txinp" hwSignal="txinp" hwResource="//@hwResources.143" required="false" visible="true"/>
  <virtualSignals name="event_lmo_137_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message137_rxinp" hwSignal="rxinp" hwResource="//@hwResources.143" required="false" visible="true"/>
  <virtualSignals name="event_lmo_138_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message138_txinp" hwSignal="txinp" hwResource="//@hwResources.144" required="false" visible="true"/>
  <virtualSignals name="event_lmo_138_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message138_rxinp" hwSignal="rxinp" hwResource="//@hwResources.144" required="false" visible="true"/>
  <virtualSignals name="event_lmo_139_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message139_txinp" hwSignal="txinp" hwResource="//@hwResources.145" required="false" visible="true"/>
  <virtualSignals name="event_lmo_139_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message139_rxinp" hwSignal="rxinp" hwResource="//@hwResources.145" required="false" visible="true"/>
  <virtualSignals name="event_lmo_140_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message140_txinp" hwSignal="txinp" hwResource="//@hwResources.146" required="false" visible="true"/>
  <virtualSignals name="event_lmo_140_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message140_rxinp" hwSignal="rxinp" hwResource="//@hwResources.146" required="false" visible="true"/>
  <virtualSignals name="event_lmo_141_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message141_txinp" hwSignal="txinp" hwResource="//@hwResources.147" required="false" visible="true"/>
  <virtualSignals name="event_lmo_141_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message141_rxinp" hwSignal="rxinp" hwResource="//@hwResources.147" required="false" visible="true"/>
  <virtualSignals name="event_lmo_142_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message142_txinp" hwSignal="txinp" hwResource="//@hwResources.148" required="false" visible="true"/>
  <virtualSignals name="event_lmo_142_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message142_rxinp" hwSignal="rxinp" hwResource="//@hwResources.148" required="false" visible="true"/>
  <virtualSignals name="event_lmo_143_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message143_txinp" hwSignal="txinp" hwResource="//@hwResources.149" required="false" visible="true"/>
  <virtualSignals name="event_lmo_143_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message143_rxinp" hwSignal="rxinp" hwResource="//@hwResources.149" required="false" visible="true"/>
  <virtualSignals name="event_lmo_144_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message144_txinp" hwSignal="txinp" hwResource="//@hwResources.150" required="false" visible="true"/>
  <virtualSignals name="event_lmo_144_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message144_rxinp" hwSignal="rxinp" hwResource="//@hwResources.150" required="false" visible="true"/>
  <virtualSignals name="event_lmo_145_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message145_txinp" hwSignal="txinp" hwResource="//@hwResources.151" required="false" visible="true"/>
  <virtualSignals name="event_lmo_145_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message145_rxinp" hwSignal="rxinp" hwResource="//@hwResources.151" required="false" visible="true"/>
  <virtualSignals name="event_lmo_146_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message146_txinp" hwSignal="txinp" hwResource="//@hwResources.152" required="false" visible="true"/>
  <virtualSignals name="event_lmo_146_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message146_rxinp" hwSignal="rxinp" hwResource="//@hwResources.152" required="false" visible="true"/>
  <virtualSignals name="event_lmo_147_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message147_txinp" hwSignal="txinp" hwResource="//@hwResources.153" required="false" visible="true"/>
  <virtualSignals name="event_lmo_147_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message147_rxinp" hwSignal="rxinp" hwResource="//@hwResources.153" required="false" visible="true"/>
  <virtualSignals name="event_lmo_148_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message148_txinp" hwSignal="txinp" hwResource="//@hwResources.154" required="false" visible="true"/>
  <virtualSignals name="event_lmo_148_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message148_rxinp" hwSignal="rxinp" hwResource="//@hwResources.154" required="false" visible="true"/>
  <virtualSignals name="event_lmo_149_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message149_txinp" hwSignal="txinp" hwResource="//@hwResources.155" required="false" visible="true"/>
  <virtualSignals name="event_lmo_149_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message149_rxinp" hwSignal="rxinp" hwResource="//@hwResources.155" required="false" visible="true"/>
  <virtualSignals name="event_lmo_150_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message150_txinp" hwSignal="txinp" hwResource="//@hwResources.156" required="false" visible="true"/>
  <virtualSignals name="event_lmo_150_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message150_rxinp" hwSignal="rxinp" hwResource="//@hwResources.156" required="false" visible="true"/>
  <virtualSignals name="event_lmo_151_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message151_txinp" hwSignal="txinp" hwResource="//@hwResources.157" required="false" visible="true"/>
  <virtualSignals name="event_lmo_151_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message151_rxinp" hwSignal="rxinp" hwResource="//@hwResources.157" required="false" visible="true"/>
  <virtualSignals name="event_lmo_152_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message152_txinp" hwSignal="txinp" hwResource="//@hwResources.158" required="false" visible="true"/>
  <virtualSignals name="event_lmo_152_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message152_rxinp" hwSignal="rxinp" hwResource="//@hwResources.158" required="false" visible="true"/>
  <virtualSignals name="event_lmo_153_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message153_txinp" hwSignal="txinp" hwResource="//@hwResources.159" required="false" visible="true"/>
  <virtualSignals name="event_lmo_153_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message153_rxinp" hwSignal="rxinp" hwResource="//@hwResources.159" required="false" visible="true"/>
  <virtualSignals name="event_lmo_154_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message154_txinp" hwSignal="txinp" hwResource="//@hwResources.160" required="false" visible="true"/>
  <virtualSignals name="event_lmo_154_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message154_rxinp" hwSignal="rxinp" hwResource="//@hwResources.160" required="false" visible="true"/>
  <virtualSignals name="event_lmo_155_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message155_txinp" hwSignal="txinp" hwResource="//@hwResources.161" required="false" visible="true"/>
  <virtualSignals name="event_lmo_155_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message155_rxinp" hwSignal="rxinp" hwResource="//@hwResources.161" required="false" visible="true"/>
  <virtualSignals name="event_lmo_156_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message156_txinp" hwSignal="txinp" hwResource="//@hwResources.162" required="false" visible="true"/>
  <virtualSignals name="event_lmo_156_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message156_rxinp" hwSignal="rxinp" hwResource="//@hwResources.162" required="false" visible="true"/>
  <virtualSignals name="event_lmo_157_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message157_txinp" hwSignal="txinp" hwResource="//@hwResources.163" required="false" visible="true"/>
  <virtualSignals name="event_lmo_157_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message157_rxinp" hwSignal="rxinp" hwResource="//@hwResources.163" required="false" visible="true"/>
  <virtualSignals name="event_lmo_158_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message158_txinp" hwSignal="txinp" hwResource="//@hwResources.164" required="false" visible="true"/>
  <virtualSignals name="event_lmo_158_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message158_rxinp" hwSignal="rxinp" hwResource="//@hwResources.164" required="false" visible="true"/>
  <virtualSignals name="event_lmo_159_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message159_txinp" hwSignal="txinp" hwResource="//@hwResources.165" required="false" visible="true"/>
  <virtualSignals name="event_lmo_159_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message159_rxinp" hwSignal="rxinp" hwResource="//@hwResources.165" required="false" visible="true"/>
  <virtualSignals name="event_lmo_160_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message160_txinp" hwSignal="txinp" hwResource="//@hwResources.166" required="false" visible="true"/>
  <virtualSignals name="event_lmo_160_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message160_rxinp" hwSignal="rxinp" hwResource="//@hwResources.166" required="false" visible="true"/>
  <virtualSignals name="event_lmo_161_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message161_txinp" hwSignal="txinp" hwResource="//@hwResources.167" required="false" visible="true"/>
  <virtualSignals name="event_lmo_161_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message161_rxinp" hwSignal="rxinp" hwResource="//@hwResources.167" required="false" visible="true"/>
  <virtualSignals name="event_lmo_162_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message162_txinp" hwSignal="txinp" hwResource="//@hwResources.168" required="false" visible="true"/>
  <virtualSignals name="event_lmo_162_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message162_rxinp" hwSignal="rxinp" hwResource="//@hwResources.168" required="false" visible="true"/>
  <virtualSignals name="event_lmo_163_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message163_txinp" hwSignal="txinp" hwResource="//@hwResources.169" required="false" visible="true"/>
  <virtualSignals name="event_lmo_163_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message163_rxinp" hwSignal="rxinp" hwResource="//@hwResources.169" required="false" visible="true"/>
  <virtualSignals name="event_lmo_164_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message164_txinp" hwSignal="txinp" hwResource="//@hwResources.170" required="false" visible="true"/>
  <virtualSignals name="event_lmo_164_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message164_rxinp" hwSignal="rxinp" hwResource="//@hwResources.170" required="false" visible="true"/>
  <virtualSignals name="event_lmo_165_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message165_txinp" hwSignal="txinp" hwResource="//@hwResources.171" required="false" visible="true"/>
  <virtualSignals name="event_lmo_165_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message165_rxinp" hwSignal="rxinp" hwResource="//@hwResources.171" required="false" visible="true"/>
  <virtualSignals name="event_lmo_166_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message166_txinp" hwSignal="txinp" hwResource="//@hwResources.172" required="false" visible="true"/>
  <virtualSignals name="event_lmo_166_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message166_rxinp" hwSignal="rxinp" hwResource="//@hwResources.172" required="false" visible="true"/>
  <virtualSignals name="event_lmo_167_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message167_txinp" hwSignal="txinp" hwResource="//@hwResources.173" required="false" visible="true"/>
  <virtualSignals name="event_lmo_167_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message167_rxinp" hwSignal="rxinp" hwResource="//@hwResources.173" required="false" visible="true"/>
  <virtualSignals name="event_lmo_168_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message168_txinp" hwSignal="txinp" hwResource="//@hwResources.174" required="false" visible="true"/>
  <virtualSignals name="event_lmo_168_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message168_rxinp" hwSignal="rxinp" hwResource="//@hwResources.174" required="false" visible="true"/>
  <virtualSignals name="event_lmo_169_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message169_txinp" hwSignal="txinp" hwResource="//@hwResources.175" required="false" visible="true"/>
  <virtualSignals name="event_lmo_169_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message169_rxinp" hwSignal="rxinp" hwResource="//@hwResources.175" required="false" visible="true"/>
  <virtualSignals name="event_lmo_170_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message170_txinp" hwSignal="txinp" hwResource="//@hwResources.176" required="false" visible="true"/>
  <virtualSignals name="event_lmo_170_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message170_rxinp" hwSignal="rxinp" hwResource="//@hwResources.176" required="false" visible="true"/>
  <virtualSignals name="event_lmo_171_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message171_txinp" hwSignal="txinp" hwResource="//@hwResources.177" required="false" visible="true"/>
  <virtualSignals name="event_lmo_171_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message171_rxinp" hwSignal="rxinp" hwResource="//@hwResources.177" required="false" visible="true"/>
  <virtualSignals name="event_lmo_172_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message172_txinp" hwSignal="txinp" hwResource="//@hwResources.178" required="false" visible="true"/>
  <virtualSignals name="event_lmo_172_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message172_rxinp" hwSignal="rxinp" hwResource="//@hwResources.178" required="false" visible="true"/>
  <virtualSignals name="event_lmo_173_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message173_txinp" hwSignal="txinp" hwResource="//@hwResources.179" required="false" visible="true"/>
  <virtualSignals name="event_lmo_173_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message173_rxinp" hwSignal="rxinp" hwResource="//@hwResources.179" required="false" visible="true"/>
  <virtualSignals name="event_lmo_174_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message174_txinp" hwSignal="txinp" hwResource="//@hwResources.180" required="false" visible="true"/>
  <virtualSignals name="event_lmo_174_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message174_rxinp" hwSignal="rxinp" hwResource="//@hwResources.180" required="false" visible="true"/>
  <virtualSignals name="event_lmo_175_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message175_txinp" hwSignal="txinp" hwResource="//@hwResources.181" required="false" visible="true"/>
  <virtualSignals name="event_lmo_175_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message175_rxinp" hwSignal="rxinp" hwResource="//@hwResources.181" required="false" visible="true"/>
  <virtualSignals name="event_lmo_176_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message176_txinp" hwSignal="txinp" hwResource="//@hwResources.182" required="false" visible="true"/>
  <virtualSignals name="event_lmo_176_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message176_rxinp" hwSignal="rxinp" hwResource="//@hwResources.182" required="false" visible="true"/>
  <virtualSignals name="event_lmo_177_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message177_txinp" hwSignal="txinp" hwResource="//@hwResources.183" required="false" visible="true"/>
  <virtualSignals name="event_lmo_177_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message177_rxinp" hwSignal="rxinp" hwResource="//@hwResources.183" required="false" visible="true"/>
  <virtualSignals name="event_lmo_178_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message178_txinp" hwSignal="txinp" hwResource="//@hwResources.184" required="false" visible="true"/>
  <virtualSignals name="event_lmo_178_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message178_rxinp" hwSignal="rxinp" hwResource="//@hwResources.184" required="false" visible="true"/>
  <virtualSignals name="event_lmo_179_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message179_txinp" hwSignal="txinp" hwResource="//@hwResources.185" required="false" visible="true"/>
  <virtualSignals name="event_lmo_179_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message179_rxinp" hwSignal="rxinp" hwResource="//@hwResources.185" required="false" visible="true"/>
  <virtualSignals name="event_lmo_180_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message180_txinp" hwSignal="txinp" hwResource="//@hwResources.186" required="false" visible="true"/>
  <virtualSignals name="event_lmo_180_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message180_rxinp" hwSignal="rxinp" hwResource="//@hwResources.186" required="false" visible="true"/>
  <virtualSignals name="event_lmo_181_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message181_txinp" hwSignal="txinp" hwResource="//@hwResources.187" required="false" visible="true"/>
  <virtualSignals name="event_lmo_181_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message181_rxinp" hwSignal="rxinp" hwResource="//@hwResources.187" required="false" visible="true"/>
  <virtualSignals name="event_lmo_182_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message182_txinp" hwSignal="txinp" hwResource="//@hwResources.188" required="false" visible="true"/>
  <virtualSignals name="event_lmo_182_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message182_rxinp" hwSignal="rxinp" hwResource="//@hwResources.188" required="false" visible="true"/>
  <virtualSignals name="event_lmo_183_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message183_txinp" hwSignal="txinp" hwResource="//@hwResources.189" required="false" visible="true"/>
  <virtualSignals name="event_lmo_183_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message183_rxinp" hwSignal="rxinp" hwResource="//@hwResources.189" required="false" visible="true"/>
  <virtualSignals name="event_lmo_184_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message184_txinp" hwSignal="txinp" hwResource="//@hwResources.190" required="false" visible="true"/>
  <virtualSignals name="event_lmo_184_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message184_rxinp" hwSignal="rxinp" hwResource="//@hwResources.190" required="false" visible="true"/>
  <virtualSignals name="event_lmo_185_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message185_txinp" hwSignal="txinp" hwResource="//@hwResources.191" required="false" visible="true"/>
  <virtualSignals name="event_lmo_185_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message185_rxinp" hwSignal="rxinp" hwResource="//@hwResources.191" required="false" visible="true"/>
  <virtualSignals name="event_lmo_186_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message186_txinp" hwSignal="txinp" hwResource="//@hwResources.192" required="false" visible="true"/>
  <virtualSignals name="event_lmo_186_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message186_rxinp" hwSignal="rxinp" hwResource="//@hwResources.192" required="false" visible="true"/>
  <virtualSignals name="event_lmo_187_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message187_txinp" hwSignal="txinp" hwResource="//@hwResources.193" required="false" visible="true"/>
  <virtualSignals name="event_lmo_187_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message187_rxinp" hwSignal="rxinp" hwResource="//@hwResources.193" required="false" visible="true"/>
  <virtualSignals name="event_lmo_188_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message188_txinp" hwSignal="txinp" hwResource="//@hwResources.194" required="false" visible="true"/>
  <virtualSignals name="event_lmo_188_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message188_rxinp" hwSignal="rxinp" hwResource="//@hwResources.194" required="false" visible="true"/>
  <virtualSignals name="event_lmo_189_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message189_txinp" hwSignal="txinp" hwResource="//@hwResources.195" required="false" visible="true"/>
  <virtualSignals name="event_lmo_189_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message189_rxinp" hwSignal="rxinp" hwResource="//@hwResources.195" required="false" visible="true"/>
  <virtualSignals name="event_lmo_190_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message190_txinp" hwSignal="txinp" hwResource="//@hwResources.196" required="false" visible="true"/>
  <virtualSignals name="event_lmo_190_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message190_rxinp" hwSignal="rxinp" hwResource="//@hwResources.196" required="false" visible="true"/>
  <virtualSignals name="event_lmo_191_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message191_txinp" hwSignal="txinp" hwResource="//@hwResources.197" required="false" visible="true"/>
  <virtualSignals name="event_lmo_191_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message191_rxinp" hwSignal="rxinp" hwResource="//@hwResources.197" required="false" visible="true"/>
  <virtualSignals name="event_lmo_192_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message192_txinp" hwSignal="txinp" hwResource="//@hwResources.198" required="false" visible="true"/>
  <virtualSignals name="event_lmo_192_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message192_rxinp" hwSignal="rxinp" hwResource="//@hwResources.198" required="false" visible="true"/>
  <virtualSignals name="event_lmo_193_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message193_txinp" hwSignal="txinp" hwResource="//@hwResources.199" required="false" visible="true"/>
  <virtualSignals name="event_lmo_193_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message193_rxinp" hwSignal="rxinp" hwResource="//@hwResources.199" required="false" visible="true"/>
  <virtualSignals name="event_lmo_194_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message194_txinp" hwSignal="txinp" hwResource="//@hwResources.200" required="false" visible="true"/>
  <virtualSignals name="event_lmo_194_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message194_rxinp" hwSignal="rxinp" hwResource="//@hwResources.200" required="false" visible="true"/>
  <virtualSignals name="event_lmo_195_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message195_txinp" hwSignal="txinp" hwResource="//@hwResources.201" required="false" visible="true"/>
  <virtualSignals name="event_lmo_195_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message195_rxinp" hwSignal="rxinp" hwResource="//@hwResources.201" required="false" visible="true"/>
  <virtualSignals name="event_lmo_196_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message196_txinp" hwSignal="txinp" hwResource="//@hwResources.202" required="false" visible="true"/>
  <virtualSignals name="event_lmo_196_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message196_rxinp" hwSignal="rxinp" hwResource="//@hwResources.202" required="false" visible="true"/>
  <virtualSignals name="event_lmo_197_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message197_txinp" hwSignal="txinp" hwResource="//@hwResources.203" required="false" visible="true"/>
  <virtualSignals name="event_lmo_197_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message197_rxinp" hwSignal="rxinp" hwResource="//@hwResources.203" required="false" visible="true"/>
  <virtualSignals name="event_lmo_198_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message198_txinp" hwSignal="txinp" hwResource="//@hwResources.204" required="false" visible="true"/>
  <virtualSignals name="event_lmo_198_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message198_rxinp" hwSignal="rxinp" hwResource="//@hwResources.204" required="false" visible="true"/>
  <virtualSignals name="event_lmo_199_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message199_txinp" hwSignal="txinp" hwResource="//@hwResources.205" required="false" visible="true"/>
  <virtualSignals name="event_lmo_199_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message199_rxinp" hwSignal="rxinp" hwResource="//@hwResources.205" required="false" visible="true"/>
  <virtualSignals name="event_lmo_200_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message200_txinp" hwSignal="txinp" hwResource="//@hwResources.206" required="false" visible="true"/>
  <virtualSignals name="event_lmo_200_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message200_rxinp" hwSignal="rxinp" hwResource="//@hwResources.206" required="false" visible="true"/>
  <virtualSignals name="event_lmo_201_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message201_txinp" hwSignal="txinp" hwResource="//@hwResources.207" required="false" visible="true"/>
  <virtualSignals name="event_lmo_201_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message201_rxinp" hwSignal="rxinp" hwResource="//@hwResources.207" required="false" visible="true"/>
  <virtualSignals name="event_lmo_202_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message202_txinp" hwSignal="txinp" hwResource="//@hwResources.208" required="false" visible="true"/>
  <virtualSignals name="event_lmo_202_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message202_rxinp" hwSignal="rxinp" hwResource="//@hwResources.208" required="false" visible="true"/>
  <virtualSignals name="event_lmo_203_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message203_txinp" hwSignal="txinp" hwResource="//@hwResources.209" required="false" visible="true"/>
  <virtualSignals name="event_lmo_203_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message203_rxinp" hwSignal="rxinp" hwResource="//@hwResources.209" required="false" visible="true"/>
  <virtualSignals name="event_lmo_204_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message204_txinp" hwSignal="txinp" hwResource="//@hwResources.210" required="false" visible="true"/>
  <virtualSignals name="event_lmo_204_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message204_rxinp" hwSignal="rxinp" hwResource="//@hwResources.210" required="false" visible="true"/>
  <virtualSignals name="event_lmo_205_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message205_txinp" hwSignal="txinp" hwResource="//@hwResources.211" required="false" visible="true"/>
  <virtualSignals name="event_lmo_205_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message205_rxinp" hwSignal="rxinp" hwResource="//@hwResources.211" required="false" visible="true"/>
  <virtualSignals name="event_lmo_206_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message206_txinp" hwSignal="txinp" hwResource="//@hwResources.212" required="false" visible="true"/>
  <virtualSignals name="event_lmo_206_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message206_rxinp" hwSignal="rxinp" hwResource="//@hwResources.212" required="false" visible="true"/>
  <virtualSignals name="event_lmo_207_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message207_txinp" hwSignal="txinp" hwResource="//@hwResources.213" required="false" visible="true"/>
  <virtualSignals name="event_lmo_207_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message207_rxinp" hwSignal="rxinp" hwResource="//@hwResources.213" required="false" visible="true"/>
  <virtualSignals name="event_lmo_208_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message208_txinp" hwSignal="txinp" hwResource="//@hwResources.214" required="false" visible="true"/>
  <virtualSignals name="event_lmo_208_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message208_rxinp" hwSignal="rxinp" hwResource="//@hwResources.214" required="false" visible="true"/>
  <virtualSignals name="event_lmo_209_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message209_txinp" hwSignal="txinp" hwResource="//@hwResources.215" required="false" visible="true"/>
  <virtualSignals name="event_lmo_209_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message209_rxinp" hwSignal="rxinp" hwResource="//@hwResources.215" required="false" visible="true"/>
  <virtualSignals name="event_lmo_210_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message210_txinp" hwSignal="txinp" hwResource="//@hwResources.216" required="false" visible="true"/>
  <virtualSignals name="event_lmo_210_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message210_rxinp" hwSignal="rxinp" hwResource="//@hwResources.216" required="false" visible="true"/>
  <virtualSignals name="event_lmo_211_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message211_txinp" hwSignal="txinp" hwResource="//@hwResources.217" required="false" visible="true"/>
  <virtualSignals name="event_lmo_211_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message211_rxinp" hwSignal="rxinp" hwResource="//@hwResources.217" required="false" visible="true"/>
  <virtualSignals name="event_lmo_212_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message212_txinp" hwSignal="txinp" hwResource="//@hwResources.218" required="false" visible="true"/>
  <virtualSignals name="event_lmo_212_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message212_rxinp" hwSignal="rxinp" hwResource="//@hwResources.218" required="false" visible="true"/>
  <virtualSignals name="event_lmo_213_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message213_txinp" hwSignal="txinp" hwResource="//@hwResources.219" required="false" visible="true"/>
  <virtualSignals name="event_lmo_213_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message213_rxinp" hwSignal="rxinp" hwResource="//@hwResources.219" required="false" visible="true"/>
  <virtualSignals name="event_lmo_214_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message214_txinp" hwSignal="txinp" hwResource="//@hwResources.220" required="false" visible="true"/>
  <virtualSignals name="event_lmo_214_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message214_rxinp" hwSignal="rxinp" hwResource="//@hwResources.220" required="false" visible="true"/>
  <virtualSignals name="event_lmo_215_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message215_txinp" hwSignal="txinp" hwResource="//@hwResources.221" required="false" visible="true"/>
  <virtualSignals name="event_lmo_215_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message215_rxinp" hwSignal="rxinp" hwResource="//@hwResources.221" required="false" visible="true"/>
  <virtualSignals name="event_lmo_216_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message216_txinp" hwSignal="txinp" hwResource="//@hwResources.222" required="false" visible="true"/>
  <virtualSignals name="event_lmo_216_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message216_rxinp" hwSignal="rxinp" hwResource="//@hwResources.222" required="false" visible="true"/>
  <virtualSignals name="event_lmo_217_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message217_txinp" hwSignal="txinp" hwResource="//@hwResources.223" required="false" visible="true"/>
  <virtualSignals name="event_lmo_217_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message217_rxinp" hwSignal="rxinp" hwResource="//@hwResources.223" required="false" visible="true"/>
  <virtualSignals name="event_lmo_218_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message218_txinp" hwSignal="txinp" hwResource="//@hwResources.224" required="false" visible="true"/>
  <virtualSignals name="event_lmo_218_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message218_rxinp" hwSignal="rxinp" hwResource="//@hwResources.224" required="false" visible="true"/>
  <virtualSignals name="event_lmo_219_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message219_txinp" hwSignal="txinp" hwResource="//@hwResources.225" required="false" visible="true"/>
  <virtualSignals name="event_lmo_219_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message219_rxinp" hwSignal="rxinp" hwResource="//@hwResources.225" required="false" visible="true"/>
  <virtualSignals name="event_lmo_220_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message220_txinp" hwSignal="txinp" hwResource="//@hwResources.226" required="false" visible="true"/>
  <virtualSignals name="event_lmo_220_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message220_rxinp" hwSignal="rxinp" hwResource="//@hwResources.226" required="false" visible="true"/>
  <virtualSignals name="event_lmo_221_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message221_txinp" hwSignal="txinp" hwResource="//@hwResources.227" required="false" visible="true"/>
  <virtualSignals name="event_lmo_221_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message221_rxinp" hwSignal="rxinp" hwResource="//@hwResources.227" required="false" visible="true"/>
  <virtualSignals name="event_lmo_222_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message222_txinp" hwSignal="txinp" hwResource="//@hwResources.228" required="false" visible="true"/>
  <virtualSignals name="event_lmo_222_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message222_rxinp" hwSignal="rxinp" hwResource="//@hwResources.228" required="false" visible="true"/>
  <virtualSignals name="event_lmo_223_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message223_txinp" hwSignal="txinp" hwResource="//@hwResources.229" required="false" visible="true"/>
  <virtualSignals name="event_lmo_223_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message223_rxinp" hwSignal="rxinp" hwResource="//@hwResources.229" required="false" visible="true"/>
  <virtualSignals name="event_lmo_224_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message224_txinp" hwSignal="txinp" hwResource="//@hwResources.230" required="false" visible="true"/>
  <virtualSignals name="event_lmo_224_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message224_rxinp" hwSignal="rxinp" hwResource="//@hwResources.230" required="false" visible="true"/>
  <virtualSignals name="event_lmo_225_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message225_txinp" hwSignal="txinp" hwResource="//@hwResources.231" required="false" visible="true"/>
  <virtualSignals name="event_lmo_225_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message225_rxinp" hwSignal="rxinp" hwResource="//@hwResources.231" required="false" visible="true"/>
  <virtualSignals name="event_lmo_226_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message226_txinp" hwSignal="txinp" hwResource="//@hwResources.232" required="false" visible="true"/>
  <virtualSignals name="event_lmo_226_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message226_rxinp" hwSignal="rxinp" hwResource="//@hwResources.232" required="false" visible="true"/>
  <virtualSignals name="event_lmo_227_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message227_txinp" hwSignal="txinp" hwResource="//@hwResources.233" required="false" visible="true"/>
  <virtualSignals name="event_lmo_227_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message227_rxinp" hwSignal="rxinp" hwResource="//@hwResources.233" required="false" visible="true"/>
  <virtualSignals name="event_lmo_228_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message228_txinp" hwSignal="txinp" hwResource="//@hwResources.234" required="false" visible="true"/>
  <virtualSignals name="event_lmo_228_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message228_rxinp" hwSignal="rxinp" hwResource="//@hwResources.234" required="false" visible="true"/>
  <virtualSignals name="event_lmo_229_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message229_txinp" hwSignal="txinp" hwResource="//@hwResources.235" required="false" visible="true"/>
  <virtualSignals name="event_lmo_229_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message229_rxinp" hwSignal="rxinp" hwResource="//@hwResources.235" required="false" visible="true"/>
  <virtualSignals name="event_lmo_230_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message230_txinp" hwSignal="txinp" hwResource="//@hwResources.236" required="false" visible="true"/>
  <virtualSignals name="event_lmo_230_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message230_rxinp" hwSignal="rxinp" hwResource="//@hwResources.236" required="false" visible="true"/>
  <virtualSignals name="event_lmo_231_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message231_txinp" hwSignal="txinp" hwResource="//@hwResources.237" required="false" visible="true"/>
  <virtualSignals name="event_lmo_231_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message231_rxinp" hwSignal="rxinp" hwResource="//@hwResources.237" required="false" visible="true"/>
  <virtualSignals name="event_lmo_232_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message232_txinp" hwSignal="txinp" hwResource="//@hwResources.238" required="false" visible="true"/>
  <virtualSignals name="event_lmo_232_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message232_rxinp" hwSignal="rxinp" hwResource="//@hwResources.238" required="false" visible="true"/>
  <virtualSignals name="event_lmo_233_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message233_txinp" hwSignal="txinp" hwResource="//@hwResources.239" required="false" visible="true"/>
  <virtualSignals name="event_lmo_233_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message233_rxinp" hwSignal="rxinp" hwResource="//@hwResources.239" required="false" visible="true"/>
  <virtualSignals name="event_lmo_234_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message234_txinp" hwSignal="txinp" hwResource="//@hwResources.240" required="false" visible="true"/>
  <virtualSignals name="event_lmo_234_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message234_rxinp" hwSignal="rxinp" hwResource="//@hwResources.240" required="false" visible="true"/>
  <virtualSignals name="event_lmo_235_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message235_txinp" hwSignal="txinp" hwResource="//@hwResources.241" required="false" visible="true"/>
  <virtualSignals name="event_lmo_235_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message235_rxinp" hwSignal="rxinp" hwResource="//@hwResources.241" required="false" visible="true"/>
  <virtualSignals name="event_lmo_236_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message236_txinp" hwSignal="txinp" hwResource="//@hwResources.242" required="false" visible="true"/>
  <virtualSignals name="event_lmo_236_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message236_rxinp" hwSignal="rxinp" hwResource="//@hwResources.242" required="false" visible="true"/>
  <virtualSignals name="event_lmo_237_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message237_txinp" hwSignal="txinp" hwResource="//@hwResources.243" required="false" visible="true"/>
  <virtualSignals name="event_lmo_237_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message237_rxinp" hwSignal="rxinp" hwResource="//@hwResources.243" required="false" visible="true"/>
  <virtualSignals name="event_lmo_238_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message238_txinp" hwSignal="txinp" hwResource="//@hwResources.244" required="false" visible="true"/>
  <virtualSignals name="event_lmo_238_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message238_rxinp" hwSignal="rxinp" hwResource="//@hwResources.244" required="false" visible="true"/>
  <virtualSignals name="event_lmo_239_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message239_txinp" hwSignal="txinp" hwResource="//@hwResources.245" required="false" visible="true"/>
  <virtualSignals name="event_lmo_239_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message239_rxinp" hwSignal="rxinp" hwResource="//@hwResources.245" required="false" visible="true"/>
  <virtualSignals name="event_lmo_240_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message240_txinp" hwSignal="txinp" hwResource="//@hwResources.246" required="false" visible="true"/>
  <virtualSignals name="event_lmo_240_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message240_rxinp" hwSignal="rxinp" hwResource="//@hwResources.246" required="false" visible="true"/>
  <virtualSignals name="event_lmo_241_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message241_txinp" hwSignal="txinp" hwResource="//@hwResources.247" required="false" visible="true"/>
  <virtualSignals name="event_lmo_241_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message241_rxinp" hwSignal="rxinp" hwResource="//@hwResources.247" required="false" visible="true"/>
  <virtualSignals name="event_lmo_242_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message242_txinp" hwSignal="txinp" hwResource="//@hwResources.248" required="false" visible="true"/>
  <virtualSignals name="event_lmo_242_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message242_rxinp" hwSignal="rxinp" hwResource="//@hwResources.248" required="false" visible="true"/>
  <virtualSignals name="event_lmo_243_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message243_txinp" hwSignal="txinp" hwResource="//@hwResources.249" required="false" visible="true"/>
  <virtualSignals name="event_lmo_243_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message243_rxinp" hwSignal="rxinp" hwResource="//@hwResources.249" required="false" visible="true"/>
  <virtualSignals name="event_lmo_244_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message244_txinp" hwSignal="txinp" hwResource="//@hwResources.250" required="false" visible="true"/>
  <virtualSignals name="event_lmo_244_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message244_rxinp" hwSignal="rxinp" hwResource="//@hwResources.250" required="false" visible="true"/>
  <virtualSignals name="event_lmo_245_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message245_txinp" hwSignal="txinp" hwResource="//@hwResources.251" required="false" visible="true"/>
  <virtualSignals name="event_lmo_245_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message245_rxinp" hwSignal="rxinp" hwResource="//@hwResources.251" required="false" visible="true"/>
  <virtualSignals name="event_lmo_246_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message246_txinp" hwSignal="txinp" hwResource="//@hwResources.252" required="false" visible="true"/>
  <virtualSignals name="event_lmo_246_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message246_rxinp" hwSignal="rxinp" hwResource="//@hwResources.252" required="false" visible="true"/>
  <virtualSignals name="event_lmo_247_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message247_txinp" hwSignal="txinp" hwResource="//@hwResources.253" required="false" visible="true"/>
  <virtualSignals name="event_lmo_247_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message247_rxinp" hwSignal="rxinp" hwResource="//@hwResources.253" required="false" visible="true"/>
  <virtualSignals name="event_lmo_248_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message248_txinp" hwSignal="txinp" hwResource="//@hwResources.254" required="false" visible="true"/>
  <virtualSignals name="event_lmo_248_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message248_rxinp" hwSignal="rxinp" hwResource="//@hwResources.254" required="false" visible="true"/>
  <virtualSignals name="event_lmo_249_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message249_txinp" hwSignal="txinp" hwResource="//@hwResources.255" required="false" visible="true"/>
  <virtualSignals name="event_lmo_249_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message249_rxinp" hwSignal="rxinp" hwResource="//@hwResources.255" required="false" visible="true"/>
  <virtualSignals name="event_lmo_250_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message250_txinp" hwSignal="txinp" hwResource="//@hwResources.256" required="false" visible="true"/>
  <virtualSignals name="event_lmo_250_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message250_rxinp" hwSignal="rxinp" hwResource="//@hwResources.256" required="false" visible="true"/>
  <virtualSignals name="event_lmo_251_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message251_txinp" hwSignal="txinp" hwResource="//@hwResources.257" required="false" visible="true"/>
  <virtualSignals name="event_lmo_251_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message251_rxinp" hwSignal="rxinp" hwResource="//@hwResources.257" required="false" visible="true"/>
  <virtualSignals name="event_lmo_252_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message252_txinp" hwSignal="txinp" hwResource="//@hwResources.258" required="false" visible="true"/>
  <virtualSignals name="event_lmo_252_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message252_rxinp" hwSignal="rxinp" hwResource="//@hwResources.258" required="false" visible="true"/>
  <virtualSignals name="event_lmo_253_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message253_txinp" hwSignal="txinp" hwResource="//@hwResources.259" required="false" visible="true"/>
  <virtualSignals name="event_lmo_253_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message253_rxinp" hwSignal="rxinp" hwResource="//@hwResources.259" required="false" visible="true"/>
  <virtualSignals name="event_lmo_254_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message254_txinp" hwSignal="txinp" hwResource="//@hwResources.260" required="false" visible="true"/>
  <virtualSignals name="event_lmo_254_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message254_rxinp" hwSignal="rxinp" hwResource="//@hwResources.260" required="false" visible="true"/>
  <virtualSignals name="event_lmo_255_txinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message255_txinp" hwSignal="txinp" hwResource="//@hwResources.261" required="false" visible="true"/>
  <virtualSignals name="event_lmo_255_rxinp" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/vs_message255_rxinp" hwSignal="rxinp" hwResource="//@hwResources.261" required="false" visible="true"/>
  <requiredApps URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/app_global_can" requiredAppName="GLOBAL_CAN" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../GLOBAL_CAN/v4_0_20/GLOBAL_CAN_0.app#/"/>
  </requiredApps>
  <hwResources name="NODE0" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_node0" resourceGroupUri="peripheral/can/0/node/0" constraintType="GLOBAL_RESOURCE" mResGrpUri="peripheral/can/0/node/0">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/can/can_0.dd#//@provided.5"/>
  </hwResources>
  <hwResources name="NODE1" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_node1" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/0/node/1"/>
  <hwResources name="NODE2" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_node2" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/0/node/2"/>
  <hwResources name="NODE3" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_node3" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/0/node/3"/>
  <hwResources name="NODE4" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_node4" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/0/node/4"/>
  <hwResources name="NODE5" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_node5" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/0/node/5"/>
  <hwResources name="MO_0" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj0" resourceGroupUri="peripheral/can/0/mo/0" constraintType="GLOBAL_RESOURCE" mResGrpUri="peripheral/can/*/mo/0">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/can/can_0.dd#//@provided.0"/>
  </hwResources>
  <hwResources name="MO_1" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj1" resourceGroupUri="peripheral/can/0/mo/1" constraintType="GLOBAL_RESOURCE" mResGrpUri="peripheral/can/*/mo/1">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/can/can_0.dd#//@provided.13"/>
  </hwResources>
  <hwResources name="MO_2" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj2" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/2"/>
  <hwResources name="MO_3" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj3" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/3"/>
  <hwResources name="MO_4" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj4" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/4"/>
  <hwResources name="MO_5" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj5" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/5"/>
  <hwResources name="MO_6" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj6" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/6"/>
  <hwResources name="MO_7" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj7" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/7"/>
  <hwResources name="MO_8" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj8" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/8"/>
  <hwResources name="MO_9" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj9" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/9"/>
  <hwResources name="MO_10" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj10" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/10"/>
  <hwResources name="MO_11" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj11" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/11"/>
  <hwResources name="MO_12" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj12" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/12"/>
  <hwResources name="MO_13" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj13" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/13"/>
  <hwResources name="MO_14" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj14" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/14"/>
  <hwResources name="MO_15" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj15" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/15"/>
  <hwResources name="MO_16" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj16" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/16"/>
  <hwResources name="MO_17" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj17" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/17"/>
  <hwResources name="MO_18" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj18" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/18"/>
  <hwResources name="MO_19" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj19" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/19"/>
  <hwResources name="MO_20" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj20" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/20"/>
  <hwResources name="MO_21" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj21" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/21"/>
  <hwResources name="MO_22" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj22" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/22"/>
  <hwResources name="MO_23" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj23" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/23"/>
  <hwResources name="MO_24" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj24" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/24"/>
  <hwResources name="MO_25" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj25" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/25"/>
  <hwResources name="MO_26" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj26" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/26"/>
  <hwResources name="MO_27" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj27" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/27"/>
  <hwResources name="MO_28" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj28" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/28"/>
  <hwResources name="MO_29" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj29" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/29"/>
  <hwResources name="MO_30" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj30" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/30"/>
  <hwResources name="MO_31" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj31" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/31"/>
  <hwResources name="MO_32" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj32" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/32"/>
  <hwResources name="MO_33" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj33" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/33"/>
  <hwResources name="MO_34" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj34" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/34"/>
  <hwResources name="MO_35" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj35" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/35"/>
  <hwResources name="MO_36" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj36" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/36"/>
  <hwResources name="MO_37" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj37" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/37"/>
  <hwResources name="MO_38" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj38" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/38"/>
  <hwResources name="MO_39" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj39" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/39"/>
  <hwResources name="MO_40" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj40" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/40"/>
  <hwResources name="MO_41" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj41" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/41"/>
  <hwResources name="MO_42" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj42" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/42"/>
  <hwResources name="MO_43" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj43" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/43"/>
  <hwResources name="MO_44" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj44" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/44"/>
  <hwResources name="MO_45" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj45" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/45"/>
  <hwResources name="MO_46" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj46" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/46"/>
  <hwResources name="MO_47" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj47" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/47"/>
  <hwResources name="MO_48" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj48" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/48"/>
  <hwResources name="MO_49" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj49" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/49"/>
  <hwResources name="MO_50" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj50" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/50"/>
  <hwResources name="MO_51" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj51" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/51"/>
  <hwResources name="MO_52" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj52" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/52"/>
  <hwResources name="MO_53" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj53" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/53"/>
  <hwResources name="MO_54" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj54" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/54"/>
  <hwResources name="MO_55" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj55" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/55"/>
  <hwResources name="MO_56" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj56" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/56"/>
  <hwResources name="MO_57" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj57" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/57"/>
  <hwResources name="MO_58" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj58" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/58"/>
  <hwResources name="MO_59" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj59" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/59"/>
  <hwResources name="MO_60" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj60" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/60"/>
  <hwResources name="MO_61" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj61" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/61"/>
  <hwResources name="MO_62" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj62" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/62"/>
  <hwResources name="MO_63" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj63" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/63"/>
  <hwResources name="MO_64" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj64" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/64"/>
  <hwResources name="MO_65" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj65" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/65"/>
  <hwResources name="MO_66" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj66" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/66"/>
  <hwResources name="MO_67" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj67" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/67"/>
  <hwResources name="MO_68" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj68" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/68"/>
  <hwResources name="MO_69" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj69" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/69"/>
  <hwResources name="MO_70" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj70" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/70"/>
  <hwResources name="MO_71" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj71" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/71"/>
  <hwResources name="MO_72" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj72" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/72"/>
  <hwResources name="MO_73" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj73" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/73"/>
  <hwResources name="MO_74" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj74" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/74"/>
  <hwResources name="MO_75" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj75" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/75"/>
  <hwResources name="MO_76" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj76" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/76"/>
  <hwResources name="MO_77" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj77" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/77"/>
  <hwResources name="MO_78" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj78" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/78"/>
  <hwResources name="MO_79" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj79" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/79"/>
  <hwResources name="MO_80" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj80" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/80"/>
  <hwResources name="MO_81" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj81" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/81"/>
  <hwResources name="MO_82" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj82" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/82"/>
  <hwResources name="MO_83" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj83" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/83"/>
  <hwResources name="MO_84" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj84" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/84"/>
  <hwResources name="MO_85" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj85" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/85"/>
  <hwResources name="MO_86" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj86" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/86"/>
  <hwResources name="MO_87" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj87" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/87"/>
  <hwResources name="MO_88" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj88" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/88"/>
  <hwResources name="MO_89" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj89" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/89"/>
  <hwResources name="MO_90" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj90" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/90"/>
  <hwResources name="MO_91" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj91" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/91"/>
  <hwResources name="MO_92" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj92" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/92"/>
  <hwResources name="MO_93" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj93" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/93"/>
  <hwResources name="MO_94" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj94" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/94"/>
  <hwResources name="MO_95" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj95" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/95"/>
  <hwResources name="MO_96" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj96" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/96"/>
  <hwResources name="MO_97" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj97" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/97"/>
  <hwResources name="MO_98" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj98" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/98"/>
  <hwResources name="MO_99" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj99" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/99"/>
  <hwResources name="MO_100" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj100" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/100"/>
  <hwResources name="MO_101" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj101" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/101"/>
  <hwResources name="MO_102" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj102" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/102"/>
  <hwResources name="MO_103" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj103" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/103"/>
  <hwResources name="MO_104" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj104" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/104"/>
  <hwResources name="MO_105" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj105" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/105"/>
  <hwResources name="MO_106" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj106" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/106"/>
  <hwResources name="MO_107" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj107" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/107"/>
  <hwResources name="MO_108" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj108" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/108"/>
  <hwResources name="MO_109" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj109" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/109"/>
  <hwResources name="MO_110" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj110" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/110"/>
  <hwResources name="MO_111" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj111" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/111"/>
  <hwResources name="MO_112" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj112" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/112"/>
  <hwResources name="MO_113" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj113" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/113"/>
  <hwResources name="MO_114" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj114" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/114"/>
  <hwResources name="MO_115" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj115" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/115"/>
  <hwResources name="MO_116" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj116" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/116"/>
  <hwResources name="MO_117" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj117" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/117"/>
  <hwResources name="MO_118" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj118" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/118"/>
  <hwResources name="MO_119" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj119" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/119"/>
  <hwResources name="MO_120" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj120" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/120"/>
  <hwResources name="MO_121" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj121" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/121"/>
  <hwResources name="MO_122" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj122" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/122"/>
  <hwResources name="MO_123" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj123" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/123"/>
  <hwResources name="MO_124" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj124" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/124"/>
  <hwResources name="MO_125" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj125" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/125"/>
  <hwResources name="MO_126" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj126" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/126"/>
  <hwResources name="MO_127" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj127" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/127"/>
  <hwResources name="MO_128" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj128" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/128"/>
  <hwResources name="MO_129" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj129" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/129"/>
  <hwResources name="MO_130" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj130" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/130"/>
  <hwResources name="MO_131" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj131" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/131"/>
  <hwResources name="MO_132" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj132" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/132"/>
  <hwResources name="MO_133" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj133" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/133"/>
  <hwResources name="MO_134" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj134" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/134"/>
  <hwResources name="MO_135" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj135" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/135"/>
  <hwResources name="MO_136" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj136" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/136"/>
  <hwResources name="MO_137" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj137" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/137"/>
  <hwResources name="MO_138" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj138" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/138"/>
  <hwResources name="MO_139" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj139" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/139"/>
  <hwResources name="MO_140" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj140" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/140"/>
  <hwResources name="MO_141" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj141" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/141"/>
  <hwResources name="MO_142" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj142" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/142"/>
  <hwResources name="MO_143" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj143" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/143"/>
  <hwResources name="MO_144" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj144" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/144"/>
  <hwResources name="MO_145" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj145" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/145"/>
  <hwResources name="MO_146" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj146" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/146"/>
  <hwResources name="MO_147" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj147" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/147"/>
  <hwResources name="MO_148" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj148" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/148"/>
  <hwResources name="MO_149" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj149" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/149"/>
  <hwResources name="MO_150" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj150" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/150"/>
  <hwResources name="MO_151" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj151" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/151"/>
  <hwResources name="MO_152" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj152" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/152"/>
  <hwResources name="MO_153" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj153" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/153"/>
  <hwResources name="MO_154" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj154" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/154"/>
  <hwResources name="MO_155" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj155" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/155"/>
  <hwResources name="MO_156" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj156" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/156"/>
  <hwResources name="MO_157" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj157" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/157"/>
  <hwResources name="MO_158" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj158" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/158"/>
  <hwResources name="MO_159" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj159" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/159"/>
  <hwResources name="MO_160" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj160" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/160"/>
  <hwResources name="MO_161" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj161" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/161"/>
  <hwResources name="MO_162" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj162" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/162"/>
  <hwResources name="MO_163" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj163" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/163"/>
  <hwResources name="MO_164" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj164" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/164"/>
  <hwResources name="MO_165" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj165" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/165"/>
  <hwResources name="MO_166" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj166" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/166"/>
  <hwResources name="MO_167" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj167" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/167"/>
  <hwResources name="MO_168" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj168" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/168"/>
  <hwResources name="MO_169" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj169" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/169"/>
  <hwResources name="MO_170" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj170" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/170"/>
  <hwResources name="MO_171" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj171" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/171"/>
  <hwResources name="MO_172" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj172" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/172"/>
  <hwResources name="MO_173" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj173" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/173"/>
  <hwResources name="MO_174" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj174" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/174"/>
  <hwResources name="MO_175" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj175" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/175"/>
  <hwResources name="MO_176" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj176" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/176"/>
  <hwResources name="MO_177" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj177" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/177"/>
  <hwResources name="MO_178" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj178" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/178"/>
  <hwResources name="MO_179" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj179" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/179"/>
  <hwResources name="MO_180" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj180" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/180"/>
  <hwResources name="MO_181" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj181" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/181"/>
  <hwResources name="MO_182" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj182" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/182"/>
  <hwResources name="MO_183" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj183" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/183"/>
  <hwResources name="MO_184" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj184" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/184"/>
  <hwResources name="MO_185" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj185" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/185"/>
  <hwResources name="MO_186" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj186" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/186"/>
  <hwResources name="MO_187" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj187" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/187"/>
  <hwResources name="MO_188" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj188" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/188"/>
  <hwResources name="MO_189" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj189" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/189"/>
  <hwResources name="MO_190" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj190" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/190"/>
  <hwResources name="MO_191" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj191" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/191"/>
  <hwResources name="MO_192" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj192" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/192"/>
  <hwResources name="MO_193" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj193" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/193"/>
  <hwResources name="MO_194" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj194" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/194"/>
  <hwResources name="MO_195" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj195" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/195"/>
  <hwResources name="MO_196" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj196" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/196"/>
  <hwResources name="MO_197" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj197" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/197"/>
  <hwResources name="MO_198" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj198" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/198"/>
  <hwResources name="MO_199" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj199" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/199"/>
  <hwResources name="MO_200" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj200" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/200"/>
  <hwResources name="MO_201" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj201" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/201"/>
  <hwResources name="MO_202" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj202" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/202"/>
  <hwResources name="MO_203" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj203" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/203"/>
  <hwResources name="MO_204" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj204" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/204"/>
  <hwResources name="MO_205" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj205" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/205"/>
  <hwResources name="MO_206" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj206" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/206"/>
  <hwResources name="MO_207" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj207" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/207"/>
  <hwResources name="MO_208" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj208" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/208"/>
  <hwResources name="MO_209" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj209" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/209"/>
  <hwResources name="MO_210" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj210" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/210"/>
  <hwResources name="MO_211" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj211" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/211"/>
  <hwResources name="MO_212" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj212" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/212"/>
  <hwResources name="MO_213" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj213" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/213"/>
  <hwResources name="MO_214" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj214" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/214"/>
  <hwResources name="MO_215" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj215" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/215"/>
  <hwResources name="MO_216" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj216" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/216"/>
  <hwResources name="MO_217" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj217" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/217"/>
  <hwResources name="MO_218" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj218" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/218"/>
  <hwResources name="MO_219" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj219" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/219"/>
  <hwResources name="MO_220" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj220" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/220"/>
  <hwResources name="MO_221" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj221" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/221"/>
  <hwResources name="MO_222" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj222" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/222"/>
  <hwResources name="MO_223" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj223" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/223"/>
  <hwResources name="MO_224" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj224" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/224"/>
  <hwResources name="MO_225" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj225" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/225"/>
  <hwResources name="MO_226" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj226" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/226"/>
  <hwResources name="MO_227" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj227" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/227"/>
  <hwResources name="MO_228" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj228" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/228"/>
  <hwResources name="MO_229" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj229" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/229"/>
  <hwResources name="MO_230" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj230" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/230"/>
  <hwResources name="MO_231" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj231" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/231"/>
  <hwResources name="MO_232" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj232" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/232"/>
  <hwResources name="MO_233" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj233" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/233"/>
  <hwResources name="MO_234" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj234" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/234"/>
  <hwResources name="MO_235" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj235" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/235"/>
  <hwResources name="MO_236" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj236" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/236"/>
  <hwResources name="MO_237" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj237" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/237"/>
  <hwResources name="MO_238" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj238" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/238"/>
  <hwResources name="MO_239" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj239" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/239"/>
  <hwResources name="MO_240" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj240" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/240"/>
  <hwResources name="MO_241" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj241" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/241"/>
  <hwResources name="MO_242" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj242" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/242"/>
  <hwResources name="MO_243" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj243" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/243"/>
  <hwResources name="MO_244" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj244" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/244"/>
  <hwResources name="MO_245" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj245" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/245"/>
  <hwResources name="MO_246" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj246" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/246"/>
  <hwResources name="MO_247" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj247" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/247"/>
  <hwResources name="MO_248" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj248" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/248"/>
  <hwResources name="MO_249" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj249" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/249"/>
  <hwResources name="MO_250" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj250" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/250"/>
  <hwResources name="MO_251" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj251" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/251"/>
  <hwResources name="MO_252" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj252" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/252"/>
  <hwResources name="MO_253" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj253" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/253"/>
  <hwResources name="MO_254" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj254" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/254"/>
  <hwResources name="MO_255" URI="http://resources/4.0.14/app/MULTICAN_CONFIG/0/hwres_can_mobj255" resourceGroupUri="" required="false" mResGrpUri="peripheral/can/*/mo/255"/>
</ResourceModel:App>
