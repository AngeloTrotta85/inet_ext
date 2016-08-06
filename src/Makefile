#
# OMNeT++/OMNEST Makefile for libinet_ext
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -I/home/angelo/Programs/omnetpp-5.0/samples/inet/src/inet/linklayer -I/home/angelo/Programs/omnetpp-5.0/samples/inet/src -I/home/angelo/Programs/omnetpp-5.0/samples/inet/src/inet/linklayer/ieee80211/mac -I/home/angelo/Programs/omnetpp-5.0/samples/inet/src/inet/physicallayer/ieee80211/packetlevel -L/home/angelo/Programs/omnetpp-5.0/samples/inet/out/$$\(CONFIGNAME\)/src -lINET -KINET_PROJ=/home/angelo/Programs/omnetpp-5.0/samples/inet
#

# Name of target to be created (-o option)
TARGET = libinet_ext$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(INET_PROJ)/src/inet/linklayer \
    -I$(INET_PROJ)/src \
    -I$(INET_PROJ)/src/inet/linklayer/ieee80211/mac \
    -I$(INET_PROJ)/src/inet/physicallayer/ieee80211/packetlevel \
    -I. \
    -Iinet \
    -Iinet/applications \
    -Iinet/applications/base \
    -Iinet/applications/udpapp \
    -Iinet/linklayer \
    -Iinet/linklayer/common \
    -Iinet/linklayer/ieee80211 \
    -Iinet/linklayer/ieee80211/mac \
    -Iinet/linklayer/ieee80211/mgmt \
    -Iinet/networklayer \
    -Iinet/networklayer/contract \
    -Iinet/networklayer/contract/ipv4 \
    -Iinet/networklayer/ipv4 \
    -Iinet/physicallayer \
    -Iinet/physicallayer/ieee80211 \
    -Iinet/physicallayer/ieee80211/packetlevel \
    -Iinet/transportlayer \
    -Iinet/transportlayer/contract \
    -Iinet/transportlayer/contract/udp \
    -Iinet/transportlayer/udp

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L$(INET_PROJ)/out/$(CONFIGNAME)/src  -lINET
LIBS += -Wl,-rpath,`abspath $(INET_PROJ)/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/inet/applications/udpapp/UDPBasicBurstExt.o \
    $O/inet/applications/udpapp/UDPStatisticsApp.o \
    $O/inet/linklayer/ieee80211/mac/DcfUpperMacExt.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhocExt.o \
    $O/inet/networklayer/ipv4/IPv4Ext.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarReceiverExt.o \
    $O/inet/transportlayer/udp/UDPExt.o \
    $O/inet/applications/base/ApplicationPacketExt_m.o \
    $O/inet/transportlayer/contract/udp/UDPDataIndicationExt_m.o

# Message files
MSGFILES = \
    inet/applications/base/ApplicationPacketExt.msg \
    inet/transportlayer/contract/udp/UDPDataIndicationExt.msg

# SM files
SMFILES =

# Other makefile variables (-K)
INET_PROJ=/home/angelo/Programs/omnetpp-5.0/samples/inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating shared library: $@
	$(Q)$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS)
	$(Q)$(SHLIB_POSTPROCESS) $O/$(TARGET)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f inet_ext inet_ext.exe libinet_ext.so libinet_ext.a libinet_ext.dll libinet_ext.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h ./*_sm.cc ./*_sm.h
	$(Q)-rm -f inet/*_m.cc inet/*_m.h inet/*_sm.cc inet/*_sm.h
	$(Q)-rm -f inet/applications/*_m.cc inet/applications/*_m.h inet/applications/*_sm.cc inet/applications/*_sm.h
	$(Q)-rm -f inet/applications/base/*_m.cc inet/applications/base/*_m.h inet/applications/base/*_sm.cc inet/applications/base/*_sm.h
	$(Q)-rm -f inet/applications/udpapp/*_m.cc inet/applications/udpapp/*_m.h inet/applications/udpapp/*_sm.cc inet/applications/udpapp/*_sm.h
	$(Q)-rm -f inet/linklayer/*_m.cc inet/linklayer/*_m.h inet/linklayer/*_sm.cc inet/linklayer/*_sm.h
	$(Q)-rm -f inet/linklayer/common/*_m.cc inet/linklayer/common/*_m.h inet/linklayer/common/*_sm.cc inet/linklayer/common/*_sm.h
	$(Q)-rm -f inet/linklayer/ieee80211/*_m.cc inet/linklayer/ieee80211/*_m.h inet/linklayer/ieee80211/*_sm.cc inet/linklayer/ieee80211/*_sm.h
	$(Q)-rm -f inet/linklayer/ieee80211/mac/*_m.cc inet/linklayer/ieee80211/mac/*_m.h inet/linklayer/ieee80211/mac/*_sm.cc inet/linklayer/ieee80211/mac/*_sm.h
	$(Q)-rm -f inet/linklayer/ieee80211/mgmt/*_m.cc inet/linklayer/ieee80211/mgmt/*_m.h inet/linklayer/ieee80211/mgmt/*_sm.cc inet/linklayer/ieee80211/mgmt/*_sm.h
	$(Q)-rm -f inet/networklayer/*_m.cc inet/networklayer/*_m.h inet/networklayer/*_sm.cc inet/networklayer/*_sm.h
	$(Q)-rm -f inet/networklayer/contract/*_m.cc inet/networklayer/contract/*_m.h inet/networklayer/contract/*_sm.cc inet/networklayer/contract/*_sm.h
	$(Q)-rm -f inet/networklayer/contract/ipv4/*_m.cc inet/networklayer/contract/ipv4/*_m.h inet/networklayer/contract/ipv4/*_sm.cc inet/networklayer/contract/ipv4/*_sm.h
	$(Q)-rm -f inet/networklayer/ipv4/*_m.cc inet/networklayer/ipv4/*_m.h inet/networklayer/ipv4/*_sm.cc inet/networklayer/ipv4/*_sm.h
	$(Q)-rm -f inet/physicallayer/*_m.cc inet/physicallayer/*_m.h inet/physicallayer/*_sm.cc inet/physicallayer/*_sm.h
	$(Q)-rm -f inet/physicallayer/ieee80211/*_m.cc inet/physicallayer/ieee80211/*_m.h inet/physicallayer/ieee80211/*_sm.cc inet/physicallayer/ieee80211/*_sm.h
	$(Q)-rm -f inet/physicallayer/ieee80211/packetlevel/*_m.cc inet/physicallayer/ieee80211/packetlevel/*_m.h inet/physicallayer/ieee80211/packetlevel/*_sm.cc inet/physicallayer/ieee80211/packetlevel/*_sm.h
	$(Q)-rm -f inet/transportlayer/*_m.cc inet/transportlayer/*_m.h inet/transportlayer/*_sm.cc inet/transportlayer/*_sm.h
	$(Q)-rm -f inet/transportlayer/contract/*_m.cc inet/transportlayer/contract/*_m.h inet/transportlayer/contract/*_sm.cc inet/transportlayer/contract/*_sm.h
	$(Q)-rm -f inet/transportlayer/contract/udp/*_m.cc inet/transportlayer/contract/udp/*_m.h inet/transportlayer/contract/udp/*_sm.cc inet/transportlayer/contract/udp/*_sm.h
	$(Q)-rm -f inet/transportlayer/udp/*_m.cc inet/transportlayer/udp/*_m.h inet/transportlayer/udp/*_sm.cc inet/transportlayer/udp/*_sm.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES) $(SM_CC_FILES)  ./*.cc inet/*.cc inet/applications/*.cc inet/applications/base/*.cc inet/applications/udpapp/*.cc inet/linklayer/*.cc inet/linklayer/common/*.cc inet/linklayer/ieee80211/*.cc inet/linklayer/ieee80211/mac/*.cc inet/linklayer/ieee80211/mgmt/*.cc inet/networklayer/*.cc inet/networklayer/contract/*.cc inet/networklayer/contract/ipv4/*.cc inet/networklayer/ipv4/*.cc inet/physicallayer/*.cc inet/physicallayer/ieee80211/*.cc inet/physicallayer/ieee80211/packetlevel/*.cc inet/transportlayer/*.cc inet/transportlayer/contract/*.cc inet/transportlayer/contract/udp/*.cc inet/transportlayer/udp/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/inet/applications/base/ApplicationPacketExt_m.o: inet/applications/base/ApplicationPacketExt_m.cc \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/INETMath.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/geometry/common/Coord.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	inet/applications/base/ApplicationPacketExt_m.h
$O/inet/applications/udpapp/UDPBasicBurstExt.o: inet/applications/udpapp/UDPBasicBurstExt.cc \
	$(INET_PROJ)/src/inet/applications/base/ApplicationBase.h \
	$(INET_PROJ)/src/inet/applications/base/ApplicationPacket_m.h \
	$(INET_PROJ)/src/inet/applications/udpapp/UDPBasicBurst.h \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/NotifierConsts.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/common/lifecycle/LifecycleOperation.h \
	$(INET_PROJ)/src/inet/common/lifecycle/NodeOperations.h \
	$(INET_PROJ)/src/inet/common/lifecycle/OperationalBase.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3AddressResolver.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IInterfaceTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoute.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo_m.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPSocket.h \
	inet/applications/udpapp/UDPBasicBurstExt.h
$O/inet/applications/udpapp/UDPStatisticsApp.o: inet/applications/udpapp/UDPStatisticsApp.cc \
	$(INET_PROJ)/src/inet/applications/base/ApplicationBase.h \
	$(INET_PROJ)/src/inet/applications/udpapp/UDPBasicBurst.h \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/INETMath.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/ModuleAccess.h \
	$(INET_PROJ)/src/inet/common/NotifierConsts.h \
	$(INET_PROJ)/src/inet/common/Units.h \
	$(INET_PROJ)/src/inet/common/geometry/common/Coord.h \
	$(INET_PROJ)/src/inet/common/geometry/common/EulerAngles.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/common/lifecycle/LifecycleOperation.h \
	$(INET_PROJ)/src/inet/common/lifecycle/NodeOperations.h \
	$(INET_PROJ)/src/inet/common/lifecycle/OperationalBase.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/AccessCategory.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/DcfUpperMac.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IFrameExchange.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IUpperMac.h \
	$(INET_PROJ)/src/inet/mobility/contract/IMobility.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3AddressResolver.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IInterfaceTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoute.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/PhysicalLayerDefs.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IModulation.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IPrintableObject.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/IIeee80211Mode.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo_m.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPSocket.h \
	inet/applications/base/ApplicationPacketExt_m.h \
	inet/applications/udpapp/UDPStatisticsApp.h \
	inet/linklayer/ieee80211/mac/DcfUpperMacExt.h \
	inet/transportlayer/contract/udp/UDPDataIndicationExt_m.h
$O/inet/linklayer/ieee80211/mac/DcfUpperMacExt.o: inet/linklayer/ieee80211/mac/DcfUpperMacExt.cc \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/DelayedInitializer.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/INETMath.h \
	$(INET_PROJ)/src/inet/common/INETUtils.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/LayeredProtocolBase.h \
	$(INET_PROJ)/src/inet/common/ModuleAccess.h \
	$(INET_PROJ)/src/inet/common/NotifierConsts.h \
	$(INET_PROJ)/src/inet/common/Units.h \
	$(INET_PROJ)/src/inet/common/geometry/common/Coord.h \
	$(INET_PROJ)/src/inet/common/geometry/common/EulerAngles.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/common/lifecycle/LifecycleOperation.h \
	$(INET_PROJ)/src/inet/common/lifecycle/NodeOperations.h \
	$(INET_PROJ)/src/inet/common/lifecycle/OperationalBase.h \
	$(INET_PROJ)/src/inet/common/mapping/Interpolation.h \
	$(INET_PROJ)/src/inet/common/mapping/MappingBase.h \
	$(INET_PROJ)/src/inet/common/mapping/MappingUtils.h \
	$(INET_PROJ)/src/inet/common/queue/IPassiveQueue.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/base/MACProtocolBase.h \
	$(INET_PROJ)/src/inet/linklayer/common/Ieee802Ctrl.h \
	$(INET_PROJ)/src/inet/linklayer/common/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/linklayer/contract/IMACProtocolControlInfo.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/AccessCategory.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/DcfUpperMac.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/DuplicateDetectors.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/FrameExchange.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/FrameExchanges.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IContention.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IDuplicateDetector.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IFragmentation.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IFrameExchange.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IMacParameters.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IMacRadioInterface.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IRateControl.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IRateSelection.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IRx.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IStatistics.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/ITx.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/ITxCallback.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/IUpperMac.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/Ieee80211Mac.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/MacParameters.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/MacPlugin.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/MacUtils.h \
	$(INET_PROJ)/src/inet/mobility/contract/IMobility.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IInterfaceTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/PhysicalLayerDefs.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/bitlevel/ISignalAnalogModel.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IAntenna.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IArrival.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IInterference.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IListening.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IListeningDecision.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IModulation.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/INoise.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IPhysicalLayer.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IPhysicalLayerFrame.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IPrintableObject.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IRadio.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IRadioFrame.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IRadioSignal.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IReceiver.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IReception.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IReceptionDecision.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IReceptionResult.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/ISNIR.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/ITransmission.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/ITransmitter.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/RadioControlInfo_m.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/IIeee80211Mode.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211Band.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211Channel.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211ModeSet.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.h \
	inet/linklayer/common/Ieee802CtrlExt.h \
	inet/linklayer/ieee80211/mac/DcfUpperMacExt.h
$O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhocExt.o: inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhocExt.cc \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/DelayedInitializer.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/INETMath.h \
	$(INET_PROJ)/src/inet/common/INETUtils.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/Units.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/Ieee802Ctrl.h \
	$(INET_PROJ)/src/inet/linklayer/common/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/linklayer/contract/IMACProtocolControlInfo.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhoc.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtBase.h \
	$(INET_PROJ)/src/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtFrames_m.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/PhysicalLayerDefs.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IModulation.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IPrintableObject.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/RadioControlInfo_m.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/IIeee80211Mode.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211Band.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211Channel.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211ModeSet.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.h \
	inet/linklayer/common/Ieee802CtrlExt.h \
	inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhocExt.h
$O/inet/networklayer/ipv4/IPv4Ext.o: inet/networklayer/ipv4/IPv4Ext.cc \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/LayeredProtocolBase.h \
	$(INET_PROJ)/src/inet/common/ModuleAccess.h \
	$(INET_PROJ)/src/inet/common/NotifierConsts.h \
	$(INET_PROJ)/src/inet/common/ProtocolMap.h \
	$(INET_PROJ)/src/inet/common/ReassemblyBuffer.h \
	$(INET_PROJ)/src/inet/common/TLVOption.h \
	$(INET_PROJ)/src/inet/common/TLVOption_m.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/common/lifecycle/LifecycleOperation.h \
	$(INET_PROJ)/src/inet/common/lifecycle/NodeOperations.h \
	$(INET_PROJ)/src/inet/common/lifecycle/NodeStatus.h \
	$(INET_PROJ)/src/inet/common/lifecycle/OperationalBase.h \
	$(INET_PROJ)/src/inet/common/queue/AbstractQueue.h \
	$(INET_PROJ)/src/inet/common/queue/QueueBase.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/Ieee802Ctrl.h \
	$(INET_PROJ)/src/inet/linklayer/common/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/linklayer/contract/IMACProtocolControlInfo.h \
	$(INET_PROJ)/src/inet/networklayer/arp/ipv4/ARPPacket_m.h \
	$(INET_PROJ)/src/inet/networklayer/common/IPProtocolId_m.h \
	$(INET_PROJ)/src/inet/networklayer/common/IPSocket.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/inet/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IARP.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IInterfaceTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/INetfilter.h \
	$(INET_PROJ)/src/inet/networklayer/contract/INetworkDatagram.h \
	$(INET_PROJ)/src/inet/networklayer/contract/INetworkProtocol.h \
	$(INET_PROJ)/src/inet/networklayer/contract/INetworkProtocolControlInfo.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoute.h \
	$(INET_PROJ)/src/inet/networklayer/contract/IRoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/contract/NetworkProtocolCommand_m.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4ControlInfo.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4ControlInfo_m.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/ICMP.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/ICMPMessage.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/ICMPMessage_m.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IIPv4RoutingTable.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4Datagram.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4Datagram_m.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4FragBuf.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/inet/networklayer/ipv4/IPv4Route.h \
	inet/linklayer/common/Ieee802CtrlExt.h \
	inet/networklayer/contract/ipv4/IPv4ControlInfoExt.h \
	inet/networklayer/ipv4/IPv4Ext.h
$O/inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarReceiverExt.o: inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarReceiverExt.cc \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/DelayedInitializer.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/INETMath.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/Units.h \
	$(INET_PROJ)/src/inet/common/geometry/common/Coord.h \
	$(INET_PROJ)/src/inet/common/geometry/common/EulerAngles.h \
	$(INET_PROJ)/src/inet/common/mapping/Interpolation.h \
	$(INET_PROJ)/src/inet/common/mapping/MappingBase.h \
	$(INET_PROJ)/src/inet/common/mapping/MappingUtils.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/mobility/contract/IMobility.h \
	$(INET_PROJ)/src/inet/physicallayer/analogmodel/packetlevel/ScalarReception.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/FlatReceiverBase.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/FlatReceptionBase.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/NarrowbandReceiverBase.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/NarrowbandReceptionBase.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/PhysicalLayerDefs.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/ReceiverBase.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/ReceptionBase.h \
	$(INET_PROJ)/src/inet/physicallayer/base/packetlevel/SNIRReceiverBase.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/bitlevel/ISignalAnalogModel.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IAntenna.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IArrival.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IErrorModel.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IInterference.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IListening.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IListeningDecision.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IModulation.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/INoise.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IPhysicalLayer.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IPhysicalLayerFrame.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IPrintableObject.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IRadio.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IRadioFrame.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IRadioSignal.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IReceiver.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IReception.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IReceptionDecision.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/IReceptionResult.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/ISNIR.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/ITransmission.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/ITransmitter.h \
	$(INET_PROJ)/src/inet/physicallayer/contract/packetlevel/RadioControlInfo_m.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/IIeee80211Mode.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211Band.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211Channel.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/mode/Ieee80211ModeSet.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/packetlevel/Ieee80211ReceiverBase.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarReceiver.h \
	$(INET_PROJ)/src/inet/physicallayer/ieee80211/packetlevel/Ieee80211TransmissionBase.h \
	inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarReceiverExt.h
$O/inet/transportlayer/contract/udp/UDPDataIndicationExt_m.o: inet/transportlayer/contract/udp/UDPDataIndicationExt_m.cc \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo_m.h \
	inet/transportlayer/contract/udp/UDPDataIndicationExt_m.h
$O/inet/transportlayer/udp/UDPExt.o: inet/transportlayer/udp/UDPExt.cc \
	$(INET_PROJ)/src/inet/common/Compat.h \
	$(INET_PROJ)/src/inet/common/INETDefs.h \
	$(INET_PROJ)/src/inet/common/InitStages.h \
	$(INET_PROJ)/src/inet/common/lifecycle/ILifecycle.h \
	$(INET_PROJ)/src/inet/features.h \
	$(INET_PROJ)/src/inet/linklayer/common/MACAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/IPProtocolId_m.h \
	$(INET_PROJ)/src/inet/networklayer/common/L3Address.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModuleIdAddress.h \
	$(INET_PROJ)/src/inet/networklayer/common/ModulePathAddress.h \
	$(INET_PROJ)/src/inet/networklayer/contract/INetworkProtocolControlInfo.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4Address.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4ControlInfo.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv4/IPv4ControlInfo_m.h \
	$(INET_PROJ)/src/inet/networklayer/contract/ipv6/IPv6Address.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo.h \
	$(INET_PROJ)/src/inet/transportlayer/contract/udp/UDPControlInfo_m.h \
	$(INET_PROJ)/src/inet/transportlayer/udp/UDP.h \
	inet/networklayer/contract/ipv4/IPv4ControlInfoExt.h \
	inet/transportlayer/contract/udp/UDPDataIndicationExt.h \
	inet/transportlayer/contract/udp/UDPDataIndicationExt_m.h \
	inet/transportlayer/udp/UDPExt.h

